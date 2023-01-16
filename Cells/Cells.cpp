#include <iostream>
#include <iomanip>
#include <windows.h>
#include <stdio.h>
#include <list>
#include "Cell.h"
using namespace std;
const int height = 20;
const int length = 40;

inline bool IsKeyDown(int Key) {
    return (GetKeyState(Key) & 0x8000) != 0;
}

void move(int xAf, int yAf, int x, int y, int curCell, Cell V, int &deaths, Cell *cells, Cell field[height][length]) {
    if (field[yAf][xAf].Type == 'p') {
        deaths++;
        cells[curCell].HP = 0;
        field[y][x] = V;
        field[yAf][xAf] = V;
    }
    else if (field[yAf][xAf].Type == 'w') {
        cells[curCell].setCurCom(2);
    }
    else if (field[yAf][xAf].Type == 'c') {
        cells[curCell].setCurCom(3);
    }
    else if (field[yAf][xAf].Type == 'f') {
        cells[curCell].HP += 10;
        field[yAf][xAf] = cells[curCell];
        field[y][x] = V;
        cells[curCell].setCurCom(4);
        cells[curCell].cordX = xAf;
        cells[curCell].cordY = yAf;
    }
    else if (field[yAf][xAf].Type == 'v') {
        field[yAf][xAf] = cells[curCell];
        field[y][x] = V;
        cells[curCell].cordX = xAf;
        cells[curCell].cordY = yAf;
        cells[curCell].setCurCom(5);
    }
}

void grab(int xAf, int yAf, int x, int y, int curCell, Cell V, Cell F, Cell* cells, Cell field[height][length]) {
    if (field[yAf][xAf].Type == 'p') {
        field[yAf][xAf] = F;
        cells[curCell].setCurCom(1);
    }
    else if (field[yAf][xAf].Type == 'w') {
        cells[curCell].setCurCom(2);
    }
    else if (field[yAf][xAf].Type == 'c') {
        cells[curCell].setCurCom(3);
    }
    else if (field[yAf][xAf].Type == 'f') {
        cells[curCell].HP += 10;
        field[y][x].HP += 10;
        field[yAf][xAf] = V;
        cells[curCell].setCurCom(4);
    }
    else if (field[yAf][xAf].Type == 'v') {
        cells[curCell].setCurCom(5);
    }
}

void watch(int xAf, int yAf, int x, int y, int curCell, Cell* cells, Cell field[height][length]) {
    if (field[yAf][xAf].Type == 'p') {
        cells[curCell].setCurCom(1);
    }
    else if (field[yAf][xAf].Type == 'w') {
        cells[curCell].setCurCom(2);
    }
    else if (field[yAf][xAf].Type == 'c') {
        cells[curCell].setCurCom(3);
    }
    else if (field[yAf][xAf].Type == 'f') {
        cells[curCell].setCurCom(4);
    }
    else if (field[yAf][xAf].Type == 'v') {
        cells[curCell].setCurCom(5);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int x, y, xAf, yAf; //переменные для массива field, 
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
    int foodCounterBoom = 3; //сколько должно пройти ходов, чтобы появилась еда
    int poisonCounter = 0; //счетчик яда
    int poisonCounterBoom = 16; //сколько должно пройти ходов, чтобы появился яд

    int counterOfCommands = 0;
    int moveDir = 0;

    int flag; //флаг для незначительных операций

    int frequency = 1000; //частота обновления

    list <int> daysAliveCycle;

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

    int choice = 0;
    //жизнь
    while (true) {
        system("cls");
        cout << "-----Меню-----" << endl;
        cout << "1. Продолжить" << endl;
        cout << "2. Сколько ходов пережили поколения" << endl;
        cout << endl;
        cin >> choice;
        
        flag = 0;

        if (choice == 2) {
            for (int d : daysAliveCycle) {
                flag++;
                cout << flag << ". " << d << endl;
            }
            system("pause");
        }
        if (choice == 1) {
            while (true) {
                if (curCell == 0) {
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
                    cout << "Текущий ход: " << turn << endl;
                    cout << "Текущий цикл: " << cycle << endl;
                    cout << "Количество смертей: " << deaths << endl;
                    int alive = 0;
                    for (int i = 0; i < 64; i++) {
                        if (cells[i].HP > 0) {
                            alive++;
                        }
                    }
                    cout << "Количество выживших: " << alive << endl;;

                    //дебаг
                    /*cout << endl;
                    for (int i = 0; i < 64; i++) {
                        for (int j = 0; j < 64; j++) {
                            cout << cells[i].gen[j] << " ";
                        }
                        cout << endl;

                    }
                    system("pause");*/
                }
                //создание мира для нового цикла
                if (deaths >= 56) {
                    daysAliveCycle.push_back(turn);
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
                                srand(time(0) + i + 1);
                                strongestCells[j].gen[rand() % 64] = rand() % 64;
                            }
                            cells[i] = strongestCells[j];
                            k = 0;
                            j++;
                        }
                        else {
                            cells[i] = strongestCells[j];
                        }
                        cells[i].HP = 20;
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
                        counterOfCommands++;
                        if (counterOfCommands == 5) {
                            break;
                        }
                    }
                }
                counterOfCommands = 0;
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
                        if (counterOfCommands == 5) {
                            break;
                        }
                    }
                }

                //ход клетки

                x = cells[curCell].cordX;
                y = cells[curCell].cordY;

                cells[curCell].HP -= 1;
                field[y][x].HP -= 1;
                counterOfCommands = 0;

                if (cells[curCell].HP < 1) {
                    deaths++;
                    field[y][x] = V;
                }
                if (cells[curCell].HP > 0) {
                    //выполнение дейсвия
                    while (true) {
                        if (counterOfCommands > 9) {
                            break;
                        }
                        //проверка на перемещение
                        if (cells[curCell].gen[cells[curCell].curCom] <= 7) {
                            moveDir = cells[curCell].gen[cells[curCell].curCom] + cells[curCell].direction;
                            if (moveDir > 7) {
                                moveDir -= 8;
                            }
                            if (moveDir == 0) {
                                move(x - 1, y - 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 1) {
                                move(x, y - 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 2) {
                                move(x + 1, y - 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 3) {
                                move(x + 1, y, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 4) {
                                move(x + 1, y + 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 5) {
                                move(x, y + 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 6) {
                                move(x - 1, y + 1, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                            if (moveDir == 7) {
                                move(x - 1, y, x, y, curCell, V, deaths, cells, field);
                                break;
                            }
                        }
                        //проверка на взятие
                        if (cells[curCell].gen[cells[curCell].curCom] <= 15) {
                            moveDir = cells[curCell].gen[cells[curCell].curCom] + cells[curCell].direction;
                            if (moveDir > 15) {
                                moveDir -= 8;
                            }
                            if (moveDir == 8) {
                                grab(x - 1, y - 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 9) {

                                grab(x, y - 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 10) {
                                grab(x + 1, y - 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 11) {
                                grab(x + 1, y, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 12) {
                                grab(x + 1, y + 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 13) {
                                grab(x, y + 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 14) {
                                grab(x - 1, y + 1, x, y, curCell, V, F, cells, field);
                                break;
                            }
                            if (moveDir == 15) {
                                grab(x - 1, y, x, y, curCell, V, F, cells, field);
                                break;
                            }
                        }
                        //проверка на просмотр
                        if (cells[curCell].gen[cells[curCell].curCom] <= 23) {
                            moveDir = cells[curCell].gen[cells[curCell].curCom] + cells[curCell].direction;
                            if (moveDir > 23) {
                                moveDir -= 8;
                            }
                            if (moveDir == 16) {
                                watch(x - 1, y - 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 17) {
                                watch(x, y - 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 18) {
                                watch(x + 1, y - 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 19) {
                                watch(x + 1, y, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 20) {
                                watch(x + 1, y + 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 21) {
                                watch(x, y + 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 22) {
                                watch(x - 1, y + 1, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                            if (moveDir == 23) {
                                watch(x - 1, y, x, y, curCell, cells, field);
                                counterOfCommands++;
                                continue;
                            }
                        }
                        //проверка на поворот
                        if (cells[curCell].gen[cells[curCell].curCom] <= 31) {
                            cells[curCell].direction += cells[curCell].gen[cells[curCell].curCom] - 24;
                            if (cells[curCell].direction > 7) {
                                cells[curCell].direction -= 8;
                            }
                            counterOfCommands++;
                            continue;
                        }
                        //проверка на переход команды
                        if (cells[curCell].gen[cells[curCell].curCom] <= 63) {
                            cells[curCell].curCom += cells[curCell].curCom;
                            if (cells[curCell].curCom > 63) {
                                cells[curCell].curCom -= 64;
                            }
                            counterOfCommands++;
                            continue;
                        }
                    }
                }



                field[cells[curCell].cordY][cells[curCell].cordX].curCom = cells[curCell].curCom;

                curCell++;
                if (curCell > 63) {
                    curCell = 0;
                }

                //завершение хода
                if (IsKeyDown(VK_F12)) {
                    break;
                }
            }
        }
    }
}