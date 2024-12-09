#include <arpa/inet.h>
#include <fcntl.h>
#include <json/json.h>
#include <json/value.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "FileChunkTask.hpp"
#include "logMessage.hpp"
#include "threadPool.hpp"
#include "util.hpp"

struct ThreadArgs {
	ThreadArgs(const std::string& ip = "127.0.0.1", uint16_t port = 8888, const std::string& filePath = "",
			   size_t startOffset = 0, size_t endOffset = 0, int num = 0)
		: _ip(ip)
		, _port(port)
		, _filePath(filePath)
		, _startOffset(startOffset)
		, _endOffset(endOffset)
		, _num(num) {}

public:
	std::string _ip;
	uint16_t _port;
	std::string _filePath;
	size_t _startOffset;
	size_t _endOffset;
	int _num;
};

static std::string account;

void mainMenu(int sock, std::unique_ptr<threadPool<FileChunkTask>>& tP);

void Usage(std::string proc) {
	std::cerr << "Usage:: \n\t" << proc << " serverIP serverPort" << std::endl;
	std::cerr << "example:: \n\t" << proc << " 127.0.0.1 8080" << std::endl;
}

bool registerAccount(int sock) {
	Json::Value ask;
	std::string account;
	std::string passwd;

	ask["ask"] = "register";
	account.clear();
	passwd.clear();
	std::cout << "请输入账号(6~16位 字母和数字) >> ";
	std::cin >> account;
	std::cout << "请输入密码(8~18位 字母和数字) >> ";
	std::cin >> passwd;

	ask["account"] = account;
	ask["passwd"] = passwd;

	std::string askStr;
	jsonUtil::serialize(ask, &askStr);

	send(sock, askStr.c_str(), askStr.size(), 0);

	std::string askResStr;
	askResStr.resize(1024);
	ssize_t s = recv(sock, (char*)askResStr.c_str(), 1024, 0);
	if (s > 0) {
		Json::Value askRes;
		jsonUtil::deserialize(askResStr, &askRes);
		if (askRes["ask"].asString() == "register" && askRes["result"].asString() == "success") {
			std::cout << "register success" << std::endl;
			return true;
		}
		else {
			std::cout << askRes["reason"].asString() << std::endl;
		}
	}

	return false;
}

bool loginAccount(int sock, std::unique_ptr<threadPool<FileChunkTask>>& tP) {
	Json::Value ask;
	std::string passwd;

	ask["ask"] = "login";
	account.clear();
	passwd.clear();
	std::cout << "请输入账号(6~16位 字母和数字) >> ";
	std::cin >> account;
	std::cout << "请输入密码(8~18位 字母和数字) >> ";
	std::cin >> passwd;

	ask["account"] = account;
	ask["passwd"] = passwd;

	std::string askStr;
	jsonUtil::serialize(ask, &askStr);

	send(sock, askStr.c_str(), askStr.size(), 0);

	std::string askResStr;
	askResStr.resize(1024);
	ssize_t s = recv(sock, (char*)askResStr.c_str(), 1024, 0);
	if (s > 0) {
		Json::Value askRes;
		jsonUtil::deserialize(askResStr, &askRes);
		if (askRes["ask"].asString() == "login" && askRes["result"].asString() == "success") {
			std::cout << "login success" << std::endl;
			tP->setOwner(account); // 设置线程池拥有者
			return true;
		}
		else {
			std::cout << askRes["reason"].asString() << std::endl;
		}
	}

	return false;
}

void loginMenu(int sock, std::unique_ptr<threadPool<FileChunkTask>>& tP) {
	int quit = false;
	while (!quit) {
		std::cout << "=============================================" << std::endl;
		std::cout << "| 1. 登录       2. 注册       0. 退出       |" << std::endl;
		std::cout << "=============================================" << std::endl;

		int option = -1;
		std::cin >> option;
		switch (option) {
		case 1:
			if (loginAccount(sock, tP)) {
				mainMenu(sock, tP);
			}
			break;
		case 2:
			registerAccount(sock);
			break;
		case 0:
			quit = true;
			break;
		}
	}
}

void showOnlineUsers(int sock) {
	Json::Value ask;
	ask["ask"] = "showOnlineUsers";

	std::string askStr;
	jsonUtil::serialize(ask, &askStr);

	send(sock, askStr.c_str(), askStr.size(), 0);

	std::string askResStr;
	askResStr.resize(1024);
	ssize_t s = recv(sock, (char*)askResStr.c_str(), askResStr.size(), 0);
	if (s > 0) {
		Json::Value askRes;
		jsonUtil::deserialize(askResStr, &askRes);

		int num = atoi(askRes["num"].asString().c_str());
		std::cout << "当前在线用户, 共 " << num << " 位:" << std::endl;
		for (int i = 1; i <= num; i++)
			std::cout << i << ": " << askRes[std::to_string(i).c_str()].asString() << std::endl;
	}
}

void* chatReader(void* args) {
	int sock = *(int*)args;

	while (true) {
		std::string message;
		message.resize(1024);
		ssize_t s = recv(sock, (char*)message.c_str(), message.size(), 0);
		if (s > 0) {
			Json::Value remoteMsg;
			jsonUtil::deserialize(message, &remoteMsg);
			if (remoteMsg["message"].asString() == "quit") {
				break;
			}
			if (remoteMsg["result"].asString() == "failed") {
				if (remoteMsg["reason"].asString() == "notfound") {
					std::cerr << "私聊开启失败, 未找到指定用户" << std::endl;
				}
				break;
			}

			std::cout << remoteMsg["account"].asString() << ":"
					  << " " << remoteMsg["message"].asString() << std::endl;
		}
	}

	return nullptr;
}

void* chatWriter(void* args) {
	std::pair<int, const std::string&>* messages = static_cast<std::pair<int, const std::string&>*>(args);
	int sock = messages->first;

	Json::Value ask;
	ask["ask"] = "privateChat";
	while (true) {
		std::string message;
		std::cin >> message;

		// 填自己的名字, 对端显示用
		ask["account"] = account.c_str();
		ask["remoteAccount"] = messages->second.c_str();
		ask["message"] = message.c_str();

		std::string askStr;
		jsonUtil::serialize(ask, &askStr);

		send(sock, askStr.c_str(), askStr.size(), 0);
		std::cout << ask["account"].asString() << ":"
				  << " " << ask["message"].asString() << std::endl;
		if (message == "quit") {
			break;
		}
	}

	return nullptr;
}

void* chatGourpWriter(void* args) {
	int sock = *(int*)args;

	Json::Value ask;
	ask["ask"] = "groupChat";

	while (true) {
		std::string message;
		std::cin >> message;

		ask["account"] = account.c_str();
		ask["message"] = message.c_str();

		std::string askStr;
		jsonUtil::serialize(ask, &askStr);
		send(sock, askStr.c_str(), askStr.size(), 0);

		if (message == "quit") {
			break;
		}
	}

	return nullptr;
}

// 两个线程 读和写
void chat(int sock) {
	showOnlineUsers(sock);

	std::string remoteAccountStr;
	remoteAccountStr.clear();
	std::cout << "请输入要私聊的用户名称:> ";
	std::cin >> remoteAccountStr;

	std::pair<int, const std::string&> messages(sock, remoteAccountStr);

	std::cout << "开始与 " << remoteAccountStr << " 聊天" << std::endl;

	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1, nullptr, chatReader, &sock);
	pthread_create(&tid2, nullptr, chatWriter, &messages);

	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);
}

void groupChat(int sock) {
	showOnlineUsers(sock);
	std::cout << "开始群聊" << std::endl;

	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1, nullptr, chatReader, &sock);
	pthread_create(&tid2, nullptr, chatGourpWriter, &sock);

	pthread_join(tid1, nullptr);

	pthread_join(tid2, nullptr);
}

void* fileUploadThread(void* args) {
	std::pair<int, std::unique_ptr<threadPool<FileChunkTask>>&>* fd_tP =
		static_cast<std::pair<int, std::unique_ptr<threadPool<FileChunkTask>>&>*>(args);

	int sock = fd_tP->first;
	std::unique_ptr<threadPool<FileChunkTask>>& tP = fd_tP->second;

	std::cout << "尝试给指定用户发送文件" << std::endl;

	showOnlineUsers(sock);
	std::cout << "请输入用户名称(请确保用户在线):> ";
	std::string targetAccount;
	std::cin >> targetAccount;
	std::cout << "请输入文件路径:> ";
	std::string targetFilePath;
	std::cin >> targetFilePath;

	// 只读打开
	int targetFd = open(targetFilePath.c_str(), O_RDONLY);
	if (targetFd < 0) {
		LOG(FATAL, "open the file %s failed! errno: %d, desc: %s", targetFilePath.c_str(), errno, strerror(errno));
		return nullptr;
	}
	// 文件大小
	size_t targetSize = lseek(targetFd, 0, SEEK_END);
	int unitChunk = 32 * 1024;
	int lastChunk = targetSize % unitChunk; // 最后一块大小
	int num = targetSize / unitChunk;
	lastChunk > 0 ? num++ : num; // 计算块数
	close(targetFd);			 // 拷贝工作不由主线程做, 所以关闭文件描述符
    LOG(DEBUG, "filesize %d, lastChunk %d, chunkNum: %d", targetSize, lastChunk, num);

	// 添加给线程池任务
	for (int i = 0; i < num; i++) {
		size_t startOffset = i * unitChunk;
		FileChunkTask fCTask(targetAccount, targetFilePath, startOffset, unitChunk, i);
		tP->pushTask(fCTask);
	}

	return nullptr;
}

// 先创建一个线程, 再由这个线程创建新的线程, 可以避免主线程等待阻塞
// 多线程传输文件, 在正式开始传输文件之前
// 要让 线程与服务器协商好 socket传输的文件范围
// 比如, 客户端 要把文件分为 4份, 4个线程分别与服务器建立新的连接
// 建立连接之后, 要先给服务器发送 这四个线程分别要拷贝哪一份, 一份多大,
// 应该从文件的什么位置开始写
void uploadFile(std::pair<int, std::unique_ptr<threadPool<FileChunkTask>>&>& fd_tP) {
	pthread_t tid;
	pthread_create(&tid, nullptr, fileUploadThread, &fd_tP);

    pthread_join(tid, nullptr);
}

void mainMenu(int sock, std::unique_ptr<threadPool<FileChunkTask>>& tP) {
	std::pair<int, std::unique_ptr<threadPool<FileChunkTask>>&> fd_tP(sock, tP);

	bool isLogin = true;

	while (isLogin) {
		std::cout << "=============================================" << std::endl;
		std::cout << "|           1. 查看登录用户列表             |" << std::endl;
		std::cout << "|           2. 选择用户私聊                 |" << std::endl;
		std::cout << "|           3. 广播聊天                     |" << std::endl;
		std::cout << "|           4. 选择用户发送文件             |" << std::endl;
		std::cout << "|           0. 退出登录                     |" << std::endl;
		std::cout << "=============================================" << std::endl;

		int option = -1;
		std::cin >> option;
		switch (option) {
		case 1:
			showOnlineUsers(sock);
			break;
		case 2:
			chat(sock);
			break;
		case 3:
			groupChat(sock);
			break;
		case 4:
			uploadFile(fd_tP);
			break;
		case 0:
			isLogin = false;
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		Usage(argv[0]);
		exit(-1);
	}

	std::unique_ptr<threadPool<FileChunkTask>> tP(threadPool<FileChunkTask>::getInstance());
	// 开启线程池
	tP->start();

	std::string serverIP = argv[1];
	uint16_t serverPort = atoi(argv[2]);

	// 创建套接字
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		// 套接字文件描述符创建失败
		LOG(FATAL, "socket() faild:: %s : %d", strerror(errno), sock);
		exit(-2); // 创建套接字失败 以 SOCKET_ERR 退出
	}
	LOG(DEBUG, "socket create success: %d", sock);

	// 填充服务器基本网络信息
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(serverPort);
	inet_aton(serverIP.c_str(), &server.sin_addr);
	if (connect(sock, (const struct sockaddr*)&server, sizeof(server)) == -1) {
		LOG(FATAL, "connect() faild:: %s : %d", strerror(errno), sock);
		exit(-3);
	}

	// 连接成功之后, 就可以向服务器发送信息了
	loginMenu(sock, tP);

	// 退出循环 客户端退出, 关闭文件描述符
	close(sock);
	tP->stop();

	return 0;
}
