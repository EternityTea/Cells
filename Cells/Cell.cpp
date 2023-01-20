#include "Cell.h"
#include <iostream>
#include <Windows.h>

using namespace std;

void Cell::setColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

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
		setColor(5, 0);
		cout << "@@";
	}
	if (Type == 'v') {
		setColor(1, 0);
		cout << "  ";
	}
	if (Type == 'f') {
		setColor(10, 0);
		cout << "()";
	}
	if (Type == 'p') {
		setColor(12, 0);
		cout << "##";
	}
	if (Type == 'c') {
		setColor(13, 0);
		cout << HP;
	}
}

void Cell::setCurCom(int x) {
	curCom += x;
	if (curCom >= 64) {
		curCom -= 64;
	}
}