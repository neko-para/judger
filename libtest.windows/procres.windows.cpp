#include "procres.h"
#include <windows.h>

long GetProcessMemUse(long handle) {
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo((HANDLE)handle, &pmc, sizeof(pmc));
	return pmc.PeakWorkingSetSize;
}