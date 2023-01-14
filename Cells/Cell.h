#pragma once
class Cell
{
public:
	char Type;
	int HP = 1;
	int ID = -1;
	int gen[64];
	int curCom = 0;
	int cordX;
	int cordY;

	Cell(char t, int id);
	Cell(char t);
	Cell();
	void print();
};