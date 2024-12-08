#include <sys/epoll.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include "logMessage.hpp"
#include "sock.hpp"

class EpollSvr {
public:
	static const int gSize = 128;
	static const int gNum = 256;
	using task_t = std::function<int(int)>;

public:
	EpollSvr(uint16_t port, task_t taskFunc)
		: _listenSock(-1)
		, _epfd(-1)
		, _port(port)
		, _taskFunc(taskFunc) {}

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
	}

	~EpollSvr() {
		if (_listenSock != -1)
			close(_listenSock);

		if (_epfd != -1)
			close(_epfd);
	}

private:
	void eventsHandler(struct epoll_event revs[], int size) {
		for (int i = 0; i < size; i++) {
			int sock = revs[i].data.fd;		 // 获取就绪的文件描述符
			uint32_t event = revs[i].events; // 获取就绪事件

			std::string clientIP;
			uint16_t clientPort;

			if (event & EPOLLIN) {
				if (sock == _listenSock) {
					int clientSock = Sock::sockAccept(_listenSock, &clientIP, &clientPort);
					if (clientSock == -1) {
						LOG(WARNING, "client accept failed! errno: %d, desc: %s", errno, strerror(errno));
						continue;
					}

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
					int ret = _taskFunc(sock);
					if (ret == 0 || ret < 0) {
						if (epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr) != 0) {
							LOG(FATAL,
								"client socket epoll events delete failed! errno: %d, "
								"desc: %s",
								errno, strerror(errno));
							exit(5);
						}
						close(sock);
						LOG(WARNING, "the client, fd: %d exit!", sock);
					}
				}
			}

			if (event & EPOLLOUT) {
			}
		}
	}

	int _listenSock;
	int _epfd;
	uint16_t _port;
	task_t _taskFunc;
};
