#include <sys/types.h>

enum CompileState {
	COMPILE_OK,
	COMPILE_ERR,
	COMPILE_TLE,
};

CompileState Compile(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, ...);
CompileState Compilev(const char* gxx, const char* src, const char* bin, size_t ml, size_t tl, char* log, const char* const arg[]);
