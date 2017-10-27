#include "test.h"
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
	vector<string> flags;
	flags.push_back("g++");
	flags.push_back(src);
	flags.push_back("-o");
	flags.push_back(bin);
	va_list lst;
	va_start(lst, log);
	char* p = va_arg(lst, char*);
	while (p) {
		flags.push_back(p);
		p = va_arg(lst, char*);
	}
	va_end(lst);
	int Pipe[2];
	pipe(Pipe);
	rlimit lim;
	lim.rlim_max = lim.rlim_cur = ml << 20;
	setrlimit(RLIMIT_AS, &lim);
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