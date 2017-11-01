#include <ncurses.h>

struct Dialog {
	WINDOW* wnd;
	bool exit;
	Dialog() {
		exit = false;
	}
	virtual ~Dialog() {
		if (wnd) {
			delwin(wnd);
		}
	}
	void Loop() {
		while (!exit) {
			Draw();
			wrefresh(wnd);
			Input(getch());
		}
		delete this;
	}
	virtual void Input(int k) {}
	virtual void Draw() {}
};