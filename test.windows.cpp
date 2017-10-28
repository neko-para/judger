#include "test.h"
#include "procres.h"
#include <vector>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <string.h>
#include <stdarg.h>
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
	while (p) {
		flags.push_back(*p);
		++p;
	}
	string cmd;
	for (unsigned i = 0; i < flags.size(); ++i) {
		cmd += flags[i] + " ";
	}
	char* cmdptr = new char[cmd.length() + 1];
	strcpy(cmdptr, cmd.c_str());

	ml <<= 20;
	tl *= CLOCKS_PER_SEC / 1000;
	HANDLE hPipeRead, hPipeWrite;
	CreatePipe(&hPipeRead, &hPipeWrite, NULL, 0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = hPipeWrite;
	CreateProcessA(gxx, cmdptr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	DWORD ret;
	clock_t base = clock();
	while (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0) {
		if (clock() - base > tl) {
			TerminateProcess(pi.hProcess, 0);
			log[0] = 0;
			return COMPILE_TLE;
		}
		if (GetProcessMemUse((long)pi.hProcess) > ml) {
			TerminateProcess(pi.hProcess, 0);
			log[0] = 0;
			return COMPILE_MLE;
		}
	}
	GetExitCodeProcess(pi.hProcess, &ret);
	if (ret) {
		DWORD rd;
		ReadFile(hPipeRead, log, 256, &rd, NULL);
		log[rd] = 0;
		return COMPILE_ERR;
	} else {
		return COMPILE_OK;
	}
}