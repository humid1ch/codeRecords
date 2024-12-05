#include "threadPool.hpp"
#include "intArithmeticTask.hpp"
#include "logMessage.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <string>
#include <unistd.h>

const std::string operators = { "+-*/\%" };

int main() {
	std::unique_ptr<threadPool<Task>> tP(threadPool<Task>::getInstance());
	// 开启线程池
	tP->start();

	srand((unsigned int)time(nullptr) ^ getpid() ^ pthread_self());

	while (true) {
		int elemOne = rand() % 20;
		int elemTwo = rand() % 10;
		char oper = operators[rand() % operators.size()];

		logMessage(NOTICE, "主线程[%lu] 派发算术任务: %d %c %d = ?", pthread_self(), elemOne, oper, elemTwo);
		Task taskTmp(elemOne, elemTwo, oper);
		tP->pushTask(taskTmp);

		usleep(100000);
	}

	return 0;
}
