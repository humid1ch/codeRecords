#include <fcntl.h>
#include <json/value.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "clientUsr.hpp"
#include "logMessage.hpp"
#include "sock.hpp"
#include "userManage.hpp"
#include "util.hpp"

#define STORAGE_PATH "/home/humid1ch/svrFile/"
static const std::string STORAGE_PATH_STR(STORAGE_PATH);

class EpollSvr {
public:
	static const int gSize = 128;
	static const int gNum = 256;

public:
	EpollSvr(uint16_t port)
		: _listenSock(-1)
		, _epfd(-1)
		, _port(port) {}

	void initEpollSvr() {
		// listen 套接字
		_listenSock = Sock::getListenSock();
		Sock::sockBind(_listenSock, _port);
		Sock::sockListen(_listenSock);
		LOG(NOTICE, "listen socket create success! fd: %d", _listenSock);

		// epoll 模型
		_epfd = epoll_create(gSize);
		if (_epfd == -1) {
			LOG(FATAL, "epoll fd create failed! errno: %d, desc: %s", errno, strerror(errno));
			exit(3);
		}
		LOG(NOTICE, "epoll fd create success! fd: %d", _epfd);
	}

	void run() {
		// 1. epoll 模型添加_listenSock
		// epoll事件添加 可读监听
		struct epoll_event ev;
		ev.events = EPOLLIN;	  // 可读
		ev.data.fd = _listenSock; // 用data(联合体)的fd

		// 给epoll模型添加 _listenSock的监听事件
		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _listenSock, &ev) != 0) {
			LOG(FATAL, "listen socket epoll events add to epoll failde! errno: %d, desc: %s", errno, strerror(errno));
			exit(4);
		}

		// 接收就绪套接字事件的时间数组
		struct epoll_event revs[gNum];
		int timeout = 1000; // 单位 毫秒
		while (true) {
			// 凡是 放进revs的 都是合法文件描述符
			int readyFdNum = epoll_wait(_epfd, revs, gNum, timeout);
			switch (readyFdNum) {
			case 0:
				// 超时, 无套接字监听事件就绪
				LOG(NOTICE, "epoll_wait timeout...");
				break;
			case -1:
				// 发生错误
				LOG(WARNING, "epoll_wait error, errno: %d, desc: %s", errno, strerror(errno));
				break;
			default:
				// 有套接字监听事件就绪
				eventsHandler(revs, readyFdNum);
				break;
			}
		}
        sleep(2);
	}

	~EpollSvr() {
		if (_listenSock != -1)
			close(_listenSock);

		if (_epfd != -1)
			close(_epfd);
	}

private:
	std::string extractFileName(const std::string& path) {
		// 找到最后一个斜杠的位置
		size_t pos = path.find_last_of("/\\"); // 提取并返回文件名

		return (pos == std::string::npos) ? path : path.substr(pos + 1);
	}

	bool showOnlineUsersHandler(int sock) {
		Json::Value users;
		int i = 1;
		for (const auto& it : _onlineClients) {
			if (it.first == sock)
				continue;

			users[std::to_string(i).c_str()] = it.second._account;
			i++;
		}
		users["num"] = std::to_string(i - 1);

		std::string usersStr;
		jsonUtil::serialize(users, &usersStr);
		send(sock, usersStr.c_str(), usersStr.size(), 0);

		return true;
	}

	bool registerHandler(const Json::Value& user) {
		bool isExist = _users.selectUser(user["account"].asString());
		if (isExist) {
			// 用户已存在
			return false;
		}

		return _users.insertUser(user);
	}

	bool loginHandler(int sock, const Json::Value& user, char* noAccount) {
		Json::Value res;
		if (!_users.selectUser(user["account"].asString(), &res)) {
			// 未找到
			*noAccount = 1;
			return false;
		}

		// 密码不匹配
		if (user["passwd"].asString() != res["passwd"].asString())
			return false;

		// 记录用户名
		auto it = _onlineClients.find(sock);
		it->second._account = user["account"].asString();

		return true;
	}

	bool privateChatHandler(int sock, const Json::Value& user) {
		std::string remoteAccount = user["remoteAccount"].asString();
		int remoteSock = -1;
		for (const auto& it : _onlineClients) {
			if (it.second._account == remoteAccount) {
				remoteSock = it.first;
			}
		}

		if (remoteSock < 0) {
			// 未找到
			Json::Value response;
			response["ask"] = "privateChat";
			response["result"] = "failed";
			response["reason"] = "notfound";
			std::string responseStr;
			jsonUtil::serialize(response, &responseStr);
			send(sock, responseStr.c_str(), responseStr.size(), 0);
			// 通知到就行了
		}

		// 找到了
		Json::Value message;
		message["account"] = user["account"].asString();
		message["message"] = user["message"].asString();
		std::string messageStr;
		jsonUtil::serialize(message, &messageStr);

		send(remoteSock, messageStr.c_str(), messageStr.size(), 0);

		return true;
	}

	bool groupCharHandler(int sock, const Json::Value& user) {
		for (const auto& it : _onlineClients) {
			if (it.first == sock) {
				continue;
			}

			Json::Value clientMsg;
			clientMsg["account"] = user["account"].asString();
			clientMsg["message"] = user["message"].asString();
			std::string clientMsgStr;
			jsonUtil::serialize(clientMsg, &clientMsgStr);

			send(it.first, clientMsgStr.c_str(), clientMsgStr.size(), 0);
		}

		return true;
	}

	bool uploadFileHandler(int sock, const Json::Value& user) {
		if (user["ask"].asString() == "uploadFile" && user["pre"].asString() == "yes") {
			// 此连接是线程池的连接
			_socksTP.insert(sock); // 记录线程池建立的套接字
			std::string account(user["owner"].asString());
			_tPSocks[account].push_back(sock); // 线程池拥有者映射套接字

			_onlineClients.erase(sock);
		}

		return true;
	}

	void eventsHandler(struct epoll_event revs[], int size) {
		for (int i = 0; i < size; i++) {
			int sock = revs[i].data.fd;		 // 获取就绪的文件描述符
			uint32_t event = revs[i].events; // 获取就绪事件

			if (event & EPOLLIN) {
				if (_socksTP.find(sock) != _socksTP.end()) {
					// 这个套接字是客户端线程池创建的套接字

					// 客户端发数据会先发 数据大小
					char sizeBuffer[512];
					memset(sizeBuffer, 0, 512 * sizeof(char));
					recv(sock, sizeBuffer, 512, 0);
					Json::Value askSize;
					jsonUtil::deserialize(std::string(sizeBuffer), &askSize);

					int fileAskSize = askSize["size"].asInt();
					if (fileAskSize == 0) {
						return;
					}
					std::cout << "fileAskSize: " << fileAskSize << std::endl;

					std::string fileDataStr;
					int cnt = 0;
					while (fileAskSize > 0) {
						cnt++;
						char buffer[1024];
						memset(buffer, 0, 1024 * sizeof(char));

						int ret = recv(sock, buffer, 1024, 0);
						if (ret > 0) {
							fileDataStr += std::string(buffer);
							fileAskSize -= ret;
						}
						else if (ret <= 0) {
							// 套接字文件描述符出问题, 不再处理数据
							LOG(WARNING, "thread pool thread socke error or exit, wrong!");
							if (epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr) != 0) {
								LOG(FATAL,
									"client socket epoll events delete failed! errno: %d, "
									"desc: %s",
									errno, strerror(errno));
								exit(5);
							}
							_onlineClients.erase(sock); // 从记录中删除
							_socksTP.erase(sock);
							close(sock);

							LOG(WARNING, "the client, fd: %d exit!", sock);
							return;
						}
						std::cout << "fileAskSize: " << fileAskSize << std::endl;
					}
					LOG(DEBUG, "END circle %d times, sock: %d, dataSize: %d", cnt, sock, fileDataStr.size());

					// 此时 fileDataStr 是完整的一个属性+chunk
					Json::Value fileData;
					if (!jsonUtil::deserialize(fileDataStr, &fileData)) {
						std::cout << "A databag from sock: " << sock << ", no json data" << std::endl;

						return;
					}

					std::string filePath(fileData["filePath"].asString());
					size_t startOffset = fileData["startOffset"].asUInt();
					std::string dataBase64(fileData["data"].asString());
					std::string data = BoostBase64::decodeBase64(dataBase64);
					LOG(DEBUG, "get chunk: filePath: %s, startOffset: %u, fileDataStrSize: %u", filePath.c_str(), startOffset,
						data.size());

					// 获取文件名, 不包括目录名的文件名
					std::string fileName = extractFileName(filePath);
					LOG(DEBUG, "target file %s", std::string(STORAGE_PATH_STR + fileName).c_str());
					int targetFd = open(std::string(STORAGE_PATH_STR + fileName).c_str(), O_RDWR | O_CREAT, 0666);
					if (targetFd < 0) {
						LOG(FATAL, "target file %s, open failed!", std::string(STORAGE_PATH_STR + fileName).c_str());
						return;
					}

					lseek(targetFd, startOffset, SEEK_SET);

					ssize_t res = write(targetFd, data.data(), data.size());
					LOG(DEBUG, "write fileData size: %d", res);
					if (res < 0) {
						LOG(FATAL, "target file %s, write failed!", fileName.c_str());
						return;
					}

					close(targetFd);
				}
				else {
					if (sock == _listenSock) {
						ClientUser client;
						int clientSock = Sock::sockAccept(_listenSock, &client);
						if (clientSock == -1) {
							LOG(WARNING, "client accept failed! errno: %d, desc: %s", errno, strerror(errno));
							continue;
						}

						// 服务器记录已连接客户端
						_onlineClients.insert(std::make_pair(clientSock, client));

						// 对客户端IO套接字, 进行读写监听
						struct epoll_event ev;
						ev.events = EPOLLIN | EPOLLOUT;
						ev.data.fd = clientSock;
						// 给epoll模型添加 clientSock的监听事件
						if (epoll_ctl(_epfd, EPOLL_CTL_ADD, clientSock, &ev) != 0) {
							LOG(FATAL,
								"client socket epoll events add to epoll failde! errno: %d, "
								"desc: %s",
								errno, strerror(errno));
							exit(4);
						}
					}
					else {
						std::string readStr;
						readStr.resize(1024);
						int ret = recv(sock, (char*)readStr.c_str(), readStr.size(), 0);
						if (ret > 0) {
							Json::Value readRes;
							// 反序列化
							jsonUtil::deserialize(readStr, &readRes);

							if (readRes["ask"].asString() == "register") {
								Json::Value regResult;
								regResult["ask"] = "register";
								std::string regResultStr;
								if (registerHandler(readRes)) {
									// 注册成功, 要反馈
									regResult["result"] = "success";
								}
								else {
									regResult["result"] = "failed";
									regResult["reason"] = "account is existed";
								}
								jsonUtil::serialize(regResult, &regResultStr);
								if (event & EPOLLOUT)
									send(sock, regResultStr.c_str(), regResultStr.size(), 0);
							}
							else if (readRes["ask"].asString() == "login") {
								char noAccount = 0;
								Json::Value loginResult;
								loginResult["ask"] = "login";
								std::string loginResultStr;
								if (loginHandler(sock, readRes, &noAccount)) {
									// 登录成功, 要反馈
									loginResult["result"] = "success";
								}
								else {
									loginResult["result"] = "failed";
									if (noAccount)
										loginResult["reason"] = "account is not exist";
									else
										loginResult["reason"] = "passwd is error";
								}
								jsonUtil::serialize(loginResult, &loginResultStr);
								if (event & EPOLLOUT)
									send(sock, loginResultStr.c_str(), loginResultStr.size(), 0);
							}
							else if (readRes["ask"].asString() == "showOnlineUsers") {
								showOnlineUsersHandler(sock);
							}
							else if (readRes["ask"].asString() == "privateChat") {
								privateChatHandler(sock, readRes);
							}
							else if (readRes["ask"].asString() == "groupChat") {
								groupCharHandler(sock, readRes);
							}
							else if (readRes["ask"].asString() == "uploadFile") {
								// 客户端线程池中的线程创建时, 会给服务器发序列化后的 json串,
								// 其中包含键值"ask"->"uploadFile" 目的是让服务器记录一下
								// 与线程池创建的套接字, 以便后续接收数据时单独处理
								uploadFileHandler(sock, readRes);
							}
						}
						if (ret == 0 || ret < 0) {
							if (epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr) != 0) {
								LOG(FATAL,
									"client socket epoll events delete failed! errno: %d, "
									"desc: %s",
									errno, strerror(errno));
								exit(5);
							}
							_onlineClients.erase(sock); // 从记录中删除
							close(sock);
							LOG(WARNING, "the client, fd: %d exit!", sock);
						}
					}
				}
			}
		}
	}

	int _listenSock;											// 服务器的监听套接字
	int _epfd;													// epoll 模型
	uint16_t _port;												// 服务器端口
	std::unordered_map<int, ClientUser> _onlineClients;			// 记录已建立连接的客户端
	ChatUsers _users;											// 数据库
	std::unordered_set<int> _socksTP;							// 记录客户端线程池建立的连接
	std::unordered_map<std::string, std::vector<int>> _tPSocks; // 记录用户有哪些线程池连接
};
