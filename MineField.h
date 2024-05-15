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


    MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize);


    int countAdjacentMines(int row, int col);

};


#endif //PROJECT3_MINEFIELD_H
