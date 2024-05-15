#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget> // Include QWidget header file
#include "MineField.h"
#include "Cell.h"

#define cellSize 30


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int numOfRows = 10;
    int numOfColumns = 10;
    int numOfMines = 5;


    QWidget *mainWidget = new QWidget;
    mainWidget->setFixedHeight(numOfRows * cellSize);
    mainWidget->setFixedWidth(numOfColumns * cellSize);
    QHBoxLayout *boxLayout = new QHBoxLayout(mainWidget);

    MineField *mineField = new MineField(numOfRows, numOfColumns, numOfMines, cellSize);
    mineField->setVerticalSpacing(0);
    mineField->setSpacing(0);


    boxLayout->addLayout(mineField); // Add the container widget to the layout

    mainWidget->setLayout(boxLayout); // Set the layout of mainWidget

    mainWidget->show();

    return QApplication::exec();
}
