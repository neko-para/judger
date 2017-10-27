#include "compiler.h"
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>

Compiler::Compiler(const char* g, size_t ml, size_t tl, const vector<string>& f) {
	gxx = g;
	memory_limit = ml;
	time_limit = tl;
	flags.push_back("g++");
	flags.push_back("");
	flags.push_back("-o");
	flags.push_back("");
	flags.insert(flags.end(), f.begin(), f.end());
}

Compiler::Compiler(const char* g, size_t ml, size_t tl, ...) {
	gxx = g;
	memory_limit = ml;
	time_limit = tl;
	flags.push_back("g++");
	flags.push_back("");
	flags.push_back("-o");
	flags.push_back("");
	va_list lst;
	va_start(lst, tl);
	char* p = va_arg(lst, char*);
	while (p) {
		flags.push_back(p);
		p = va_arg(lst, char*);
	}
	va_end(lst);
}

Compiler::State Compiler::Compile(const string& file, const string& binary, string& log) {
	int Pipe[2];
	pipe(Pipe);
	rlimit lim;
	lim.rlim_max = lim.rlim_cur = memory_limit * 1024 * 1024;
	setrlimit(RLIMIT_AS, &lim);
	flags[1] = file;
	flags[3] = binary;
	pid_t Sub = fork();
	if (Sub) {
		int ret;
		int cnt = 0;
		while (Sub != waitpid(Sub, &ret, WNOHANG)) {
			usleep(100 * 1000);
			if (++cnt == time_limit) {
				kill(Sub, SIGKILL);
				return COMPILE_TLE;
			}
		}
		close(Pipe[1]);
		char buffer[257];
		buffer[read(Pipe[0], buffer, 256)] = 0;
		log = buffer;
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
		execv(gxx.c_str(), Arg.data());
		return COMPILE_OK;
	}
}