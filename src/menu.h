#include <string>
#include <functional>
#include <vector>
using std::string;
using std::function;
using std::vector;


struct Menu {
	string title;
	function<void(Menu*)> proc;
	vector<Menu*> items;
	Menu* father;
	Menu(const char* t, const function<void(Menu*)>& p, ...) : title(t), proc(p), father(0) {
		va_list lst;
		va_start(lst, p);
		Menu* ptr = va_arg(lst, Menu*);
		while (ptr) {
			items.push_back(ptr);
			ptr->father = this;
			ptr = va_arg(lst, Menu*);
		}
		va_end(lst);
	}
	size_t GetWidth() const {
		size_t l = 0;
		for (size_t i = 0; i < items.size(); ++i) {
			l = std::max(l, items[i]->title.length());
		}
		return l + 1;
	}
	size_t GetPos() const {
		if (father) {
			return std::find(father->items.begin(), father->items.end(), this) - father->items.begin();
		} else {
			return 0;
		}
	}
	void draw(WINDOW* win, size_t& x, size_t& y) {
		if (!father) {
			for (size_t i = 0; i < items.size(); ++i) {
				wmove(win, i, 0);
				waddstr(win, items[i]->title.c_str());
			}
			x = GetWidth();
			y = 0;
		} else {
			father->draw(win, x, y);
			y += GetPos();
			for (size_t i = 0; i < items.size(); ++i) {
				wmove(win, y + i, x);
				waddstr(win, items[i]->title.c_str());
			}
			x += GetWidth();
		}
	}
	Menu* Translate(int& x, int& y) {
		if (!father) {
			--y;
			if (x < GetWidth() && y < items.size()) {
				return items[y];
			} else {
				x -= GetWidth();
				return 0;
			}
		} else {
			Menu* t = father->Translate(x, y);
			if (t) {
				return t;
			}
			if (x < 0 || y < 0) {
				return 0;
			}
			y -= GetPos();
			if (x < GetWidth() && y < items.size()) {
				return items[y];
			} else {
				x -= GetWidth();
				return 0;
			}
		}
	}
};