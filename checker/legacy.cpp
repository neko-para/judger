#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool Check(const string& a, const string& b, string& log) {
	
}

int main(int argc, char* argv[]) {
	ifstream A(argv[1]);
	ifstream B(argv[2]);
	string lA, lB;
	string log;
	while (true) {
		bool hA = (bool)getline(A, lA);
		bool hB = (bool)getline(B, lB);
		if (hA) {
			if (hB) {
				if (!Check(lA, lB, log)) {
					cout << log << endl;
					return 1;
				}
			} else {
				cout << "Output is longer than answer" << endl;
				return 1;
			}
		} else {
			if (hB) {
				cout << "Output is shorter than answer" << endl;
				return 1;
			} else {
				break;
			}
		}
	}
	return 0;
}