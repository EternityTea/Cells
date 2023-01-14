#include "Cell.h"
#include <iostream>

using namespace std;

Cell::Cell(char t, int id) {
	Type = t;
	ID = id;
	srand(time(0) + ID);
	for (int i = 0; i < 64; i++) {
		gen[i] = rand() % 64;
	}
}

Cell::Cell(char t) {
	Type = t;
}

Cell::Cell() {
	Type = 'v';
}

void Cell::print() {
	if (Type == 'w') {
		cout << "@@";
	}
	if (Type == 'v') {
		cout << "__";
	}
	if (Type == 'f') {
		cout << "()";
	}
	if (Type == 'p') {
		cout << "##";
	}
	if (Type == 'c') {
		cout << HP;
	}
}