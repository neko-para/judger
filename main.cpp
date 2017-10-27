#include "compiler.h"
#include <iostream>

int main() {
	Compiler compiler("/usr/bin/gcc", "-std=c++11", 0);
	string log;
	int ret = compiler.Compile("test.cpp", "test", log);
	std::cout << ret << ' ' << log << std::endl;
}