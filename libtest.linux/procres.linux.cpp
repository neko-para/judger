#include "procres.h"
#include <stdio.h>
#include <sys/types.h>

long GetProcessMemUse(long handle) {
	pid_t pid = (pid_t)handle;
	char buffer[256];
	sprintf(buffer, "/proc/%d/statm", pid);
	FILE* f = fopen(buffer, "r");
	long l;
	fscanf(f, "%ld", &l);
	fscanf(f, "%ld", &l);
	fclose(f);
	return l << 12;
}