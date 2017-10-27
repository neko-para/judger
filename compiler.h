#ifndef _JUDGER_COMPILER_H_
#define _JUDGER_COMPILER_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

class Compiler {
private:
	string gxx;
	vector<string> flags;
public:
	enum State {
		COMPILE_OK,
		COMPILE_ERR,
		COMPILE_TLE,
	};

	Compiler(const string& gxx, const vector<string>& flags);
	Compiler(const char* gxx, ...);
	State Compile(const string& file, const string& binary, string& log);
};

#endif