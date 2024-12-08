#include "threadPool.hpp"
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include "FileChunkTask.hpp"
#include "logMessage.hpp"

const std::string operators = { "+-*/\%" };

int main() {
	std::unique_ptr<threadPool<FileChunkTask>> tP(threadPool<FileChunkTask>::getInstance());
	// 开启线程池
	tP->start();

	while (true) {

		tP->pushTask();

		usleep(100000);
	}

	return 0;
}
