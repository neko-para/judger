#ifndef _JUDGER_COMPILER_H_
#define _JUDGER_COMPILER_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

class Compiler {
private:
	string gxx;
	size_t memory_limit;
	size_t time_limit;
	vector<string> flags;
public:
	enum State {
		COMPILE_OK,
		COMPILE_ERR,
		COMPILE_TLE,
	};

	Compiler(const char* gxx, size_t ml, size_t tl, const vector<string>& flags);
	Compiler(const char* gxx, size_t ml, size_t tl, ...);
	State Compile(const string& file, const string& binary, string& log);
};

#endif