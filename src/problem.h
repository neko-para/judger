#include <sys/types.h>
#include <string>
#include "dialog.h"
using std::string;

struct Problem {
	string name;
	size_t time;
	size_t memory;
	size_t count;
};

struct AddProblemDialog : public Dialog {
	AddProblemDialog(Problem& problem) {
		wnd = newwin(Width, Height, 0, 0);
		
	}
};