#include <iostream>
#include <utility>

class RRefTest {
public:
	RRefTest() {}
	RRefTest(const RRefTest& object) {
		std::cout << "copy constructor";
	}
	RRefTest(RRefTest&& moveObject) {
		std::cout << "move constructor";
	}
};

template <class T>
void test(T& test) {
	std::cout << "你好&" << std::endl;
}

template <class T>
void test(T&& test) {
	std::cout << "你好&&" << std::endl;
}

int main() {
	RRefTest a;

	RRefTest& aR = a;
	RRefTest&& aRR = std::move(a);

	test(a);
	test(aR);
	test(aRR);
	test(std::move(a));

	return 0;
}
