#include <functional>
#include <iostream>
#include <tuple>

void loopPrint(int& num) {
	while (num < 10) {
		num++;
		std::cout << num << " " << std::endl;
	}
}

int main() {
	int a = 1;
	std::tuple<int> elems{ a };


	return 0;
}
