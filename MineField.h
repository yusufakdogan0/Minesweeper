#ifndef PROJECT3_MINEFIELD_H
#define PROJECT3_MINEFIELD_H

#include "Cell.h"
#include <QGridLayout>

class MineField : public QGridLayout{

    Q_OBJECT

public:
    // total number of rows, columns and mines that minefield has
    int numOfRows;
    int numOfColumns;
    int numOfMines;
    // The 2D matrix that stores cells
    std::vector<std::vector<Cell*>> widgetMatrix;
    // Total score which is the number of opened cells
    int score;
    // Size of a cell
    int cellSize;
    // if hint is pressed hint cell will indicate which cell is hinted
    Cell *hint_cell;

    // Constructor
    MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize);
    // Functions
    void reveal(int row, int col);
    void onRightClick(int row, int col);
    int countAdjacentMines(int row, int col);
    void endGame();
    std::vector<Cell*> adjacentUnrevealedCells(Cell *cell);
    Cell* findSafeCell(Cell *cell);
};


#endif //PROJECT3_MINEFIELD_H
