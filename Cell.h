#ifndef PROJECT3_CELL_H
#define PROJECT3_CELL_H


#include <QPushButton>

class Cell : public QPushButton{
public :
    int row;
    int column;
    int status;
    bool opened;
    int buttonSize;
    Cell(int cellSize, int row, int column);

};


#endif //PROJECT3_CELL_H
