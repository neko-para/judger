#include "test.h"
#include <iostream>

int main() {
	char log[257];
	int ret = Compile("/usr/bin/g++", "123.cpp", "123", 256, 5000, log, "-std=c++11", 0);
	std::cout << ret << ' ' << log << std::endl;
	ret = Run("123", 256, 1000);
	std::cout << ret << std::endl;
	ret = Check("checker/legacy", ".output", ".output", log);
	std::cout << ret << ' ' << log << std::endl;
}