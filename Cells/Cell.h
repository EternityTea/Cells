#pragma once
#include <Windows.h>
class Cell
{
public:
	char Type;
	int HP = 20;
	int ID = -1;
	int gen[64];
	int curCom = 0;
	int cordX;
	int cordY;
	int direction = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	Cell(char t, int id);
	Cell(char t);
	Cell();
	void print();
	void setCurCom(int x);
	void setColor(int text, int bg);
};