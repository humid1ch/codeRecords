#include <iostream>
#include <string>

class ConstTest {
public:
	ConstTest(const std::string name = "xuxuxu")
		: _name(name) {}

	void print() {
		std::cout << _name << std::endl;
	}

	void sprintc() const {
		std::cout << "hahaha" << std::endl;
	}

private:
	const std::string _name;
};

int main() {
	ConstTest ct;
	ct.print();

	return 0;
}
