#include "../libtest/test.h"
#include "../libtest/procres.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
using std::vector;
using std::string;
using std::max;

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
	tl *= CLOCKS_PER_SEC / 1000;
	int Pipe[2];
	pipe(Pipe);
	pid_t Sub = fork();
	if (Sub) {
		int ret;
		clock_t base = clock();
		while (Sub != waitpid(Sub, &ret, WNOHANG)) {
			if (clock() - base > tl) {
				kill(Sub, SIGKILL);
				log[0] = 0;
				return COMPILE_TLE;
			}
			if (GetProcessMemUse(Sub) > ml) {
				kill(Sub, SIGKILL);
				log[0] = 0;
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

RunState Run(const char* program, size_t ml, size_t tl, size_t* mu, size_t* tu) {
	ml <<= 20;
	tl *= CLOCKS_PER_SEC / 1000 + 1000;
	pid_t Sub = fork();
	*mu = 0;
	if (Sub) {
		int ret;
		clock_t base = clock();
		while (Sub != waitpid(Sub, &ret, WNOHANG)) {
			if (clock() - base > tl) {
				kill(Sub, SIGKILL);
				return RUN_TLE;
			}
			size_t m = GetProcessMemUse(Sub);
			if (m > ml) {
				kill(Sub, SIGKILL);
				return RUN_MLE;
			}
			*mu = max(*mu, m);
		}
		*tu = (clock() - base) * 1000 / CLOCKS_PER_SEC;
		if (*tu > tl - 1000) {
			return RUN_TLE;
		}
		if (WEXITSTATUS(ret)) {
			return RUN_RE;
		} else {
			return RUN_OK;
		}
	} else {
		int in = open(".input", O_RDONLY);
		int out = open(".output", O_WRONLY);
		dup2(in, 0);
		dup2(out, 1);
		execl(program, program, NULL);
		return RUN_OK;
	}
}

CheckState Check(const char* program, const char* fileA, const char* fileB, const char* param, char* log) {
	vector<string> flags;
	flags.push_back(program);
	flags.push_back(fileA);
	flags.push_back(fileB);
	flags.push_back(param);
	int Pipe[2];
	pipe(Pipe);
	pid_t Sub = fork();
	if (Sub) {
		int ret;
		while (Sub != wait(&ret)) {}
		close(Pipe[1]);
		log[read(Pipe[0], log, 256)] = 0;
		char buffer[256];
		while (read(Pipe[0], buffer, 256)) {}
		return (CheckState)WEXITSTATUS(ret);
	} else {
		vector<char*> Arg;
		for (size_t i = 0; i < flags.size(); ++i) {
			char* p = new char[flags[i].length() + 1];
			strcpy(p, flags[i].c_str());
			Arg.push_back(p);
		}
		Arg.push_back(0);
		close(Pipe[0]);
		dup2(Pipe[1], 1);
		execv(program, Arg.data());
		return CHECK_OK;
	}
}