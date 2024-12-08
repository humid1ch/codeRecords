#ifndef __SELECT_SVR_HPP__
#define __SELECT_SVR_HPP__

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "logMessage.hpp"

class Sock {
public:
	static const int gBackLog = 20;

	static int getListenSock() {
		int listenSock = socket(PF_INET, SOCK_STREAM, 0);
		if (listenSock == -1) {
			LOG(FATAL, "listenSock create failed!");
			exit(-1);
		}
		LOG(NOTICE, "listenSock create success!");

		// 设置地址和端口复用, 用于防止服务端关闭时, 还存在连接
		// 导致服务器不能及时重启
		int opt = 1;
		setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (const void *)&opt, sizeof(opt));

		return listenSock;
	}

	static void sockBind(int socket, uint16_t port) {
		struct sockaddr_in local;
		memset(&local, 0, sizeof(struct sockaddr_in));

		local.sin_family = PF_INET;
		local.sin_port = htons(port);
		local.sin_addr.s_addr = INADDR_ANY;

		if (bind(socket, (const struct sockaddr *)&local, sizeof(local)) == -1) {
			LOG(FATAL, "sock create failed!");
			exit(-2);
		}

		LOG(NOTICE, "sock create success!");
	}

	static void sockListen(int socket) {
		if (listen(socket, gBackLog) == -1) {
			LOG(FATAL, "listenSock listen failed!");
			exit(-3);
		}

		LOG(NOTICE, "listenSock listen success!");
	}

	static int sockAccept(int listenSock, std::string *clientIP, uint16_t *clientPort) {
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);

		int serviceSock = accept(listenSock, (struct sockaddr *)&peer, &len);
		if (serviceSock == -1) {
			LOG(WARNING, "connect failed!");
			return -1;
		}

		// 携带客户端IP和端口
		if (clientIP != nullptr)
			*clientIP = ntohs(peer.sin_port);
		if (clientPort != nullptr)
			*clientIP = inet_ntoa(peer.sin_addr);

		LOG(NOTICE, "connect success!");

		return serviceSock;
	}
};

#endif
