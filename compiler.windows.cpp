#include "compiler.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string.h>

Compiler::State Compiler::Compile(const string& file, const string& binary, string& log) {
	flags[1] = file;
	flags[3] = binary;
	string cmd;
	for (unsigned i = 0; i < flags.size(); ++i) {
		cmd += flags[i] + " ";
	}
	char* cmdptr = new char[cmd.length() + 1];
	strcpy(cmdptr, cmd.c_str());
	std::cerr << cmd << std::endl;

	HANDLE hPipeRead, hPipeWrite;
	CreatePipe(&hPipeRead, &hPipeWrite, NULL, 0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = hPipeWrite;
	CreateProcessA(gxx.c_str(), cmdptr, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	DWORD ret;
	int cnt = 0;
	while (WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0) {
		Sleep(100);
		if (++cnt == time_limit) {
			TerminateProcess(pi.hProcess, 0);
			return COMPILE_TLE;
		}
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc));
		if (pmc.PeakWorkingSetSize > (memory_limit << 20)) {
			TerminateProcess(pi.hProcess, 0);
			return COMPILE_ERR;
		}
	}
	GetExitCodeProcess(pi.hProcess, &ret);
	if (ret) {
		char buffer[257];
		DWORD rd;
		ReadFile(hPipeRead, buffer, 256, &rd, NULL);
		buffer[rd] = 0;
		log = buffer;
		return COMPILE_ERR;
	} else {
		return COMPILE_OK;
	}
}