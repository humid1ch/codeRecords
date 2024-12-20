#include <iostream>

template <class Type>
Type add(Type a, Type b) {
	return a + b;
}

template <class Type>
Type max(const Type& a, const Type& b, const Type& c) {
	return a + b;
}

float max(const float& a, const float& b) {
	return a > b ? a : b;
}

int max(const int& a, const int& b) {
	return a > b ? a : b;
}

int main() {
	std::cout << max(1.6f, 1.2f) << std::endl;

	return 0;
}
