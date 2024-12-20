#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

class ParaTest {
private:
	ParaTest(const ParaTest&) = delete;
};

// void print1(int n, std::mutex& mtx) {
// 	std::thread::id tid = std::this_thread::get_id();
//
// 	for (int i = 0; i < n; ++i) {
// 		mtx.lock();
// 		std::cout << tid << ": " << i << std::endl;
// 		mtx.unlock();
// 		std::this_thread::sleep_for(std::chrono::milliseconds(100));
// 	}
// }

void print1(int n, int& count) {
	std::thread::id tid = std::this_thread::get_id();

	for (int i = 0; i < n; ++i) {
		std::cout << tid << ": " << i << std::endl;
		count++;
	}
}

int main() {
	// std::mutex mtx;
	int cnt = 0;
	// 	std::thread t1(print1, 10000, std::ref(cnt)); // 这里传值 总是传值拷贝
	// 	std::thread t2(print1, 10000, std::ref(cnt));

	std::thread t1([&]() {
		while (cnt < 100) {
			cnt++;
			std::cout << std::this_thread::get_id() << ": " << cnt << std::endl;
		}
	});

	std::thread t2([&]() {
		while (cnt < 100) {
			cnt++;
			std::cout << std::this_thread::get_id() << ": " << cnt << std::endl;
		}
	});

	t1.join();
	t2.join();

	std::cout << cnt << std::endl;

	return 0;
}
