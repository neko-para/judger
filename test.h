#include <sys/types.h>

enum CompileState {
	COMPILE_OK,
	COMPILE_ERR,
	COMPILE_TLE,
	COMPILE_MLE,
};

CompileState Compile(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, ...);
CompileState Compilev(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, const char* const arg[]);

enum RunState {
	RUN_OK,
	RUN_RE,
	RUN_TLE,
	RUN_MLE,
};

RunState Run(const char* program, size_t ml, size_t tl);

enum CheckState {
	CHECK_OK,
	CHECK_WA,
	CHECK_PE,
};

CheckState Check(const char* program, const char* fileA, const char* fileB, char* log);