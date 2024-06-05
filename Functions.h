// This file only exists to let the usage of main functions from other files
#ifndef PROJECT3_FUNCTIONS_H
#define PROJECT3_FUNCTIONS_H

#include "MineField.h"
#include <QMainWindow>

void updateScore(MineField *minefield);
void checkWin(MineField *minefield);
void loseGame(MineField *minefield);
void initializeGame();
void restartGame(QMainWindow *mainWindow);
void hint(MineField *minefield);


#endif //PROJECT3_FUNCTIONS_H
