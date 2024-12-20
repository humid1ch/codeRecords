#include <atomic>
#include <cstdio>
#include <iostream>

using AtomicInt = std::atomic<int>;

class test {
public:
	test() {}

private:
	test& _t1;
};

int main() {
	int cnt = 1;
	cnt++;

	AtomicInt cntt(1);
	cntt++;

	std::cout << cntt << std::endl;

	return 0;
}
