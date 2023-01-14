#include <iostream>
#include <iomanip>
#include <windows.h>
#include <stdio.h>
#include "Cell.h"
using namespace std;
const int height = 20;
const int length = 40;

inline bool IsKeyDown(int Key) {
    return (GetKeyState(Key) & 0x8000) != 0;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int x, y; //переменные для массива field, 
    Cell field[height][length];

    Cell V('v'); //клетка пустого места
    Cell W('w'); //клетка стены
    Cell F('f'); //клетка еды
    Cell P('p'); //клетка яда

    int curCell = 0; //Какая по счету клетка ходит
    int turn = 0; //Номер хода
    int cycle = 0; //Текущий цикл
    int deaths = 0; //Сколько клеток погибло

    int foodCounter = 0; //счетчик еды
    int foodCounterBoom = 4; //сколько должно пройти ходов, чтобы появилась еда
    int poisonCounter = 0; //счетчик яда
    int poisonCounterBoom = 8; //сколько должно пройти ходов, чтобы появился яд

    int flag; //флаг для незначительных операций

    //создание карты
    for (int i = 0; i < height; i++) { 
        for (int j = 0; j < length; j++) {
            field[i][j] = V;
            if (i == 0 || j == 0 || i == height - 1 || j == length - 1) {
                field[i][j] = W;
            }
        }
    }

    Cell cells[64];
    Cell strongestCells[8];

    //создание массива клеток
    for (int i = 0; i < 64; i++) {
        cells[i] = Cell('c', i);
    }

    //размещение клеток
    for (int i = 0; i < 64; i++) {
        while (true) {
            x = 1 + rand() % length - 2;
            y = 1 + rand() % height - 2;
            if (field[y][x].Type == 'v') {
                cells[i].cordX = x;
                cells[i].cordY = y;
                field[y][x] = cells[i];
                break;
            }
        }
    }
    //размещение стартовой еды
    for (int i = 0; i < 64; i++) {
        while (true) {
            x = 1 + rand() % length - 2;
            y = 1 + rand() % height - 2;
            if (field[y][x].Type == 'v') {
                field[y][x].Type = 'f';
                break;
            }
        }
    }

    //печать генома всех клеток
    /*for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            cout << cells[i].gen[j] << " ";
        }
        cout << endl;
    }*/

    //жизнь
    while (true) {
        while (true) {
            //создание мира для нового цикла
            if (deaths >= 56) {
                cycle++;
                turn = 0;
                deaths = 0;
                //выделение 8 сильнейших из клеток
                for (int i = 0, j = 0; i < 64; i++) {
                    if (cells[i].HP > 0) {
                        strongestCells[j] = cells[i];
                        j++;
                    }
                }
                //создание нового массива клеток из потомков 8 предыдущих
                for (int i = 0, j = 0, k = 1; i < 64; i++, k++) {
                    if (k == 8) {
                        srand(time(0) + i);
                        flag = 1 + rand() % 3;
                        for (int x = 0; x < flag; x++) {
                            strongestCells[j].gen[rand() % 64] = rand() % 64;
                        }
                        k = 0;
                        j++;
                    }
                    else {
                        cells[i] = strongestCells[j];
                    }
                    cells[i].HP = 1;
                    cells[i].ID = i;
                    cells[i].curCom = 0;
                }
                //обновление карты
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < length; j++) {
                        field[i][j] = V;
                        if (i == 0 || j == 0 || i == height - 1 || j == length - 1) {
                            field[i][j] = W;
                        }
                    }
                }
                //размщение клеток
                for (int i = 0; i < 64; i++) {
                    while (true) {
                        x = 1 + rand() % length - 2;
                        y = 1 + rand() % height - 2;
                        if (field[y][x].Type == 'v') {
                            cells[i].cordX = x;
                            cells[i].cordY = y;
                            field[y][x] = cells[i];
                            break;
                        }
                    }
                }
                //размещение стартовой еды
                for (int i = 0; i < 64; i++) {
                    while (true) {
                        x = 1 + rand() % length - 2;
                        y = 1 + rand() % height - 2;
                        if (field[y][x].Type == 'v') {
                            field[y][x].Type = 'f';
                            break;
                        }
                    }
                }
            }
            //конец создания мира для нового цикла

            while (true) {
                if (cells[curCell].HP < 1) {
                    curCell++;
                    if (curCell > 63) {
                        curCell = 0;
                    }
                }
                else {
                    break;
                }
            }

            //добавление еды
            foodCounter++;
            if (foodCounter == foodCounterBoom) {
                while (true) {
                    x = 1 + rand() % length - 2;
                    y = 1 + rand() % height - 2;
                    if (field[y][x].Type == 'v') {
                        field[y][x].Type = 'f';
                        foodCounter = 0;
                        break;
                    }
                }
            }

            //добавление яда
            poisonCounter++;
            if (poisonCounter == poisonCounterBoom) {
                while (true) {
                    x = 1 + rand() % length - 2;
                    y = 1 + rand() % height - 2;
                    if (field[y][x].Type == 'v') {
                        field[y][x].Type = 'p';
                        poisonCounter = 0;
                        break;
                    }
                }
            }

            //ход клетки
            cells[curCell].HP -= 1;
            field[cells[curCell].cordY][cells[curCell].cordX].HP -= 1;

            if (cells[curCell].HP < 1) {
                deaths++;
                field[cells[curCell].cordY][cells[curCell].cordX] = V;
            }

            curCell++;
            if (curCell > 63) {
                curCell = 0;
            }
            turn++;

            //рисование экрана
            system("cls");
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < length; j++) {
                    //cout << setw(2);
                    cout.width(2);
                    field[i][j].print();
                }
                cout << endl;
            }
            cout << "Текущий ход: " << turn;
            cout << "Текущий цикл: " << cycle;
            cout << "Количество смертей: " << deaths;

            //завершение хода
            Sleep(50);
            if (IsKeyDown(VK_F12)) {
                break;
            }
        }
        system("pause");
    }

}