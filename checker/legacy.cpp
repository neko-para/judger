#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
	ifstream A(argv[1]);
	ifstream B(argv[2]);
	if (B.fail()) {
		cout << "File error";
		return 3;
	}
	string sa, sb;
	while (A >> sa) {
		if (!(B >> sb)) {
			cout << "Shorter than answer";
			return 1;
		} else {
			if (sa != sb) {
				cout << "Expect " << sa << ", get " << sb;
				return 1;
			}
		}
	}
	if (B >> sb) {
		cout << "Longer than answer";
		return 1;
	}
	A.clear();
	B.clear();
	A.seekg(ios::beg);
	B.seekg(ios::beg);
	char ac, bc;
	while (true) {
		ac = A.get();
		bc = B.get();
		if (ac == ' ' && (A.peek() == '\n' || A.peek() == EOF)) {
			ac = A.get();
		}
		if (bc == ' ' && (B.peek() == '\n' || B.peek() == EOF)) {
			bc = B.get();
		}
		if ((ac == EOF && bc == '\n' && B.peek() == EOF) || (ac == '\n' && bc == EOF && A.peek() == EOF) || (ac == EOF && bc == EOF)) {
			break;
		}
		if (ac == EOF || bc == EOF) {
			cout << "Presentation error";
			return 2;
		}
		if (isgraph(ac) != isgraph(bc)) {
			cout << "Presentation error";
			return 2;
		} else if (isgraph(ac)) {
			continue;
		} else if (ac != bc) {
			cout << "Presentation error";
			return 2;
		}
	}
	return 0;
}