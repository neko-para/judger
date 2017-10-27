#include "test.h"
#include <iostream>

int main() {
	char log[257];
	int ret = Compile("/usr/bin/g++", "123.cpp", "123", 256, 50, log, "-std=c++11", 0);
	std::cout << ret << ' ' << log << std::endl;
}