#ifndef PROJECT3_MINEFIELD_H
#define PROJECT3_MINEFIELD_H

#include "Cell.h"
#include <QGridLayout>

class MineField : public QGridLayout{

    Q_OBJECT

public:
    int numOfRows;
    int numOfColumns;
    int numOfMines;
    std::vector<std::vector<Cell*>> widgetMatrix;
    int score;
    int cellSize;


    MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize);
    bool winCheck();
    void reveal(int row, int col);
    void onRightClick(int row, int col);
    int countAdjacentMines(int row, int col);
    void endGame();

};


#endif //PROJECT3_MINEFIELD_H
