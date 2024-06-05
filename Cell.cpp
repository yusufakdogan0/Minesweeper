#include "Cell.h"
#include <QApplication>
#include <QPushButton>

// Constructs a default cell in given coordinates
Cell::Cell(int cellSize, int row, int column){
    this->row = row;
    this->column = column;
    this->status = 0;
    opened = false;
    flagged = false;
    virtual_flag = false;
    this->buttonSize = cellSize;
    this->setFixedSize(buttonSize,buttonSize);
}
