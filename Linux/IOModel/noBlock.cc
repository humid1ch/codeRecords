#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "util.hpp"

using func_t = std::function<void()>;

int main() {
	std::vector<func_t> funcs;
	funcs.push_back([]() {
		std::cout << "other task1 is running" << std::endl;
	});
	funcs.push_back([]() {
		std::cout << "other task2 is running" << std::endl;
	});
	funcs.push_back([]() {
		std::cout << "other task3 is running" << std::endl;
	});

	Util::setNoBlock(0);

	char buffer[1024] = { 0 };
	while (true) {
		int n = scanf("%s", buffer);
		if (n == -1) {
			std::cout << "errno: " << errno << ", desc: " << std::strerror(errno) << std::endl;
			for (const auto& f : funcs)
				f();
		}
		else
			std::cout << "some data: " << buffer << std::endl;

		sleep(1);
	}

	return 0;
}
