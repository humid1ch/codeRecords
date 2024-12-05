#include "selectSvr.hpp"

#include <bits/types/struct_timeval.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

static void usage(const std::string &process) {
	std::cerr << "\nUsage: " << process << " port" << std::endl;
}

// 用于记录已连接文件描述符
#define DFL -1
std::vector<int> fdsArray(sizeof(fd_set) * 8, DFL);
// 大小为 sizeof(fd_set) * 8, 每个单元内容为DFL的vector

static void handlerEvent(int listenSock,
						 fd_set &readFds,
						 std::unordered_map<int, std::pair<std::string, uint16_t>> &clientsMap,
						 std::string *clientIP = nullptr,
						 uint16_t *clientPort = nullptr) {
	if (FD_ISSET(listenSock, &readFds)) {
		int sock = SelectSock::sockAccept(listenSock, clientIP, clientPort);
		if (sock == -1) {
			// 连接建立失败
			return;
		}

		int i = 0;
		for (; i < fdsArray.size(); i++) {
			if (fdsArray[i] == DFL)
				break;
		}
		if (i == fdsArray.size()) {
			LOG(WARNING, "server fd is full, unable to create new connetation!");
			// 关闭文件描述符
			close(sock);
		}
		else {
			// 记录文件描述符
			fdsArray[sock] = sock;
			// 新连接 存储到clientsMap中
			if (clientIP == nullptr || clientPort == nullptr) {
				clientsMap.insert(std::make_pair(sock, std::make_pair("", 0)));
				LOG(NOTICE, "new connetc, fd: %d, ip: noknow, port: noknow", sock);
			}
			else {
				clientsMap.insert(std::make_pair(sock, std::make_pair(*clientIP, *clientPort)));
				LOG(NOTICE, "new connetc, fd: %d, ip: %s, port: %d", sock, clientIP->c_str(), *clientPort);
			}
		}
	}

	for (int i = 1; i < fdsArray.size(); i++) {
		if (fdsArray[i] == DFL)
			continue;

		if (FD_ISSET(fdsArray[i], &readFds)) {
			// 某个连接的文件描述符读取就绪了
            // 处理
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		usage(argv[0]);

		exit(1);
	}

	Log log;
	log.enable();

	int listenSock = SelectSock::getListenSock();
	SelectSock::sockBind(listenSock, atoi(argv[1]));
	SelectSock::sockListen(listenSock);
	fdsArray[0] = listenSock;

	std::unordered_map<int, std::pair<std::string, uint16_t>> clientsMap;  // 存储已连接的客户端socket, ip, port

	while (true) {
		std::string clientIP;
		uint16_t clientPort;

		// select 的fds输入输出型, 所以每次select 都要设置
		int maxFd = listenSock;
		fd_set readFds;
		FD_ZERO(&readFds);
		for (int i = 0; i < fdsArray.size(); i++) {
			if (fdsArray[i] == DFL)
				continue;

			maxFd < fdsArray[i] ? maxFd = fdsArray[i] : maxFd;	// 找fd最大值
			// 文件描述符集 设置所有已连接的socket
			FD_SET(fdsArray[i], &readFds);
		}
		// 设置监听套接字
		FD_SET(listenSock, &readFds);
		struct timeval selecTimeout = { 1, 0 };	 // 用于设置超时时间 s, ms, 空指针永久阻塞
		int ret = select(maxFd + 1, &readFds, nullptr, nullptr, &selecTimeout);

		switch (ret) {
		case 0:
			std::cout << "select timeout..." << std::endl;
			break;
		case -1:
			// 有文件描述符异常, 比如关闭
			// 需要做一些处理, 比如取消客户端的文件描述符
			for (int i = 0; i < fdsArray.size(); i++) {
				if (fdsArray[i] == DFL)
					continue;

				// 有异常
				// 	if (FD_ISSET(fdsArray[i], &exceptFds)) {
				// 		clientsMap.erase(fdsArray[i]);	// 删除客户端记录
				// 		fdsArray[i] = DFL; // 删除文件描述符记录
				// 	}
			}

			std::cerr << "errno: " << errno << std::strerror(errno) << std::endl;
			break;
		default:
			// 不能直接accept, 因为accept也是阻塞的, 不符合多路转接特点,
			// 要把阻塞和IO分开执行 等待成功, 再accept
			handlerEvent(listenSock, readFds, clientsMap);

			break;
		}
	}

	return 0;
}
