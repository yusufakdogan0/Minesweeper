#ifndef PROJECT3_CELL_H
#define PROJECT3_CELL_H


#include <QPushButton>

class Cell : public QPushButton{
public :
    // row and column represents coordinates of the cell in minefield
    int row;
    int column;
    // status = -1 if this is a mine
    // otherwise status = number of adjacent mines
    int status;
    // true if the cell is flagged
    bool flagged;
    // true if cell is opened
    bool opened;
    // size of the cell on window
    int buttonSize;
    // Used for hint
    bool virtual_flag;
    Cell(int cellSize, int row, int column);

};


#endif //PROJECT3_CELL_H
