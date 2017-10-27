#include "compiler.h"
#include <stdarg.h>

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