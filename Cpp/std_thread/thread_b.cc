#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

void loopPrint(int& cnt) {
	// while (cnt < 10) {
		std::cout << "thread_" << std::this_thread::get_id() << ", out:" << cnt << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	// }
}

int main() {
	int cnt = 6;
	std::thread td1(loopPrint, cnt);

	td1.join();

	std::cout << "主线程, cnt: " << cnt << std::endl;

	return 0;
}
