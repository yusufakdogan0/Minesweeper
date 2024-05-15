#ifndef PROJECT3_FUNCTIONS_H
#define PROJECT3_FUNCTIONS_H

#include "MineField.h"
#include <QMainWindow>

void updateScore(MineField *minefield);
void checkWin(MineField *minefield);
void loseGame(MineField *minefield);
void initializeGame();
void restartGame(QMainWindow *mainWindow);


#endif //PROJECT3_FUNCTIONS_H