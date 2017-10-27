#include "test.h"
#include <vector>
#include <string>
#include <windows.h>
#include <psapi.h>
#include <string.h>
#include <stdarg.h>
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
	string cmd;
	for (unsigned i = 0; i < flags.size(); ++i) {
		cmd += flags[i] + " ";
	}
	char* cmdptr = new char[cmd.length() + 1];
	strcpy(cmdptr, cmd.c_str());

	HANDLE hPipeRead, hPipeWrite;
	CreatePipe(&hPipeRead, &hPipeWrite, NULL, 0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = hPipeWrite;
	CreateProcessA(gxx, cmdptr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	DWORD ret;
	int cnt = 0;
	while (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0) {
		Sleep(100);
		if (++cnt == tl) {
			TerminateProcess(pi.hProcess, 0);
			return COMPILE_TLE;
		}
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc));
		if (pmc.PeakWorkingSetSize > (ml << 20)) {
			TerminateProcess(pi.hProcess, 0);
			return COMPILE_ERR;
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