#include "epollSvr.hpp"
#include <unistd.h>
#include <memory>

static void usage(const std::string& process) {
	std::cerr << "\nUsage: \r" << process << " port" << std::endl;
}

int readStr(int fd) {
	char buffer[1024];
	int ret = recv(fd, (void*)buffer, sizeof(buffer) - 1, 0);
	if (ret > 0) {
		buffer[ret] = 0;
		LOG(DEBUG, "from fd: %d, read: %s", fd, buffer);
	}

	return ret;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		usage(argv[0]);
		exit(-1);
	}

	std::unique_ptr<EpollSvr> epollSvr(new EpollSvr(atoi(argv[1]), readStr));

	epollSvr->initEpollSvr();

	epollSvr->run();

	return 0;
}
