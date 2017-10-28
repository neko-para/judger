#include "test.h"
#include "procres.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
using std::vector;
using std::string;

CompileState Compile(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, ...) {
	vector<const char*> flags;
	va_list lst;
	va_start(lst, log);
	char* p = va_arg(lst, char*);
	while (p) {
		flags.push_back(p);
		p = va_arg(lst, char*);
	}
	va_end(lst);
	return Compilev(gxx, src, bin, ml, tl, log, flags.data());
}

CompileState Compilev(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, const char* const arg[]) {
	vector<string> flags;
	flags.push_back("g++");
	flags.push_back(src);
	flags.push_back("-o");
	flags.push_back(bin);
	const char* const* p = arg;
	while (*p) {
		flags.push_back(*p);
		++p;
	}
	ml <<= 20;
	int Pipe[2];
	pipe(Pipe);
	pid_t Sub = fork();
	if (Sub) {
		int ret;
		int cnt = 0;
		while (Sub != waitpid(Sub, &ret, WNOHANG)) {
			usleep(100 * 1000);
			if (++cnt == tl) {
				kill(Sub, SIGKILL);
				return COMPILE_TLE;
			}
			if (GetProcessMemUse(Sub) > ml) {
				kill(Sub, SIGKILL);
				return COMPILE_MLE;
			}
		}
		close(Pipe[1]);
		log[read(Pipe[0], log, 256)] = 0;
		char buffer[256];
		while (read(Pipe[0], buffer, 256)) {}
		if (WEXITSTATUS(ret)) {
			return COMPILE_ERR;
		} else {
			return COMPILE_OK;
		}
	} else {
		vector<char*> Arg;
		for (size_t i = 0; i < flags.size(); ++i) {
			char* p = new char[flags[i].length() + 1];
			strcpy(p, flags[i].c_str());
			Arg.push_back(p);
		}
		Arg.push_back(0);
		close(Pipe[0]);
		dup2(Pipe[1], 2);
		execv(gxx, Arg.data());
		return COMPILE_OK;
	}
}