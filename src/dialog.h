#include <string>
#include <string.h>
#include <ncurses.h>
using std::string;

extern int Width, Height;

struct Dialog {
	WINDOW* wnd;
	bool exit;
	long ret;
	Dialog() {
		exit = false;
		ret = 0;
	}
	virtual ~Dialog() {
		if (wnd) {
			delwin(wnd);
		}
	}
	long Loop() {
		while (!exit) {
			Draw();
			wrefresh(wnd);
			Input(getch());
		}
		delete this;
		return ret;
	}
	virtual void Input(int k) {}
	virtual void Draw() {}
};

struct InputDialog : public Dialog {
	string& str;
	InputDialog(const char* msg, string& str) : str(str) {
		wnd = newwin(5, Width >> 1, (Height - 5) >> 1, Width >> 2);
		box(wnd, 0, 0);
		int l = strlen(msg);
		mvwaddstr(wnd, 1, ((Width >> 1) - l) >> 1, msg);
		wrefresh(wnd);
		curs_set(1);
		noraw();
		nocbreak();
		echo();
	}
	~InputDialog() {
		raw();
		cbreak();
		curs_set(0);
		noecho();
	}
	long Loop() {
		static char buffer[1024];
		mvgetstr((Height >> 1) + 1, (Width >> 2) + 1, buffer);
		str = buffer;
		delete this;
		return 0;
	}
};