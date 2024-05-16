#ifndef PROJECT3_CELL_H
#define PROJECT3_CELL_H


#include <QPushButton>

class Cell : public QPushButton{
public :
    int row;
    int column;
    int status;
    bool flagged;
    bool opened;
    int buttonSize;
    bool virtual_flag;
    Cell(int cellSize, int row, int column);

};


#endif //PROJECT3_CELL_H
