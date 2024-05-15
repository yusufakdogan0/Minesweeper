#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget> // Include QWidget header file
#include <QMessageBox>
#include "MineField.h"
#include "Cell.h"
#include "Functions.h"

#define cellSize 30
#define numOfRows 3
#define numOfColumns 3
#define numOfMines 3

QLabel *score_label;


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    initializeGame();


    return QApplication::exec();
}

void updateScore(MineField *minefield){
    QString text = "Score: " + QString::number(minefield->score);
    score_label->setText(text);
}

void checkWin(MineField *minefield){
    if(minefield->score + numOfMines == numOfColumns * numOfRows){
        minefield->endGame();
        QMessageBox *messageBox = new QMessageBox;
        messageBox->setText("You Win!");
        messageBox->show();
    }
}
void loseGame(MineField *minefield){
    minefield->endGame();
    QMessageBox *messageBox = new QMessageBox;
    messageBox->setText("You Lose!");
    messageBox->show();
}
void initializeGame(){
    // Create main window
    QMainWindow *mainWindow = new QMainWindow;



    // Create minefield
    QWidget *mainWidget = new QWidget;
    mainWidget->setFixedHeight(numOfRows * cellSize + 50);
    mainWidget->setFixedWidth(numOfColumns * cellSize + 20);
    QVBoxLayout *mainBox = new QVBoxLayout;
    QHBoxLayout *horizontalBox = new QHBoxLayout;

    // Create labels and buttons
    QPushButton *restartButton = new QPushButton;
    restartButton->setText("Restart");
    QObject::connect(restartButton, &QPushButton::clicked, [=]() {
        restartGame(mainWindow);
    });
    QPushButton *hintButton = new QPushButton;
    hintButton->setText("Hint");

    score_label = new QLabel;
    score_label->setText("Score: 0");
    horizontalBox->addWidget(score_label);
    horizontalBox->addWidget(restartButton);
    horizontalBox->addWidget(hintButton);

    mainBox->addLayout(horizontalBox);



    MineField *mineField = new MineField(numOfRows, numOfColumns, numOfMines, cellSize);
    mineField->setVerticalSpacing(0);
    mineField->setSpacing(0);


    mainBox->addLayout(mineField); // Add the container widget to the layout
    mainWidget->setLayout(mainBox);


    mainWindow->setCentralWidget(mainWidget);
    mainWindow->setWindowTitle("Minesweeper");
    QIcon icon(R"(..\assets\mine.png)");
    mainWindow->setWindowIcon(icon);
    mainWindow->show();
}
void restartGame(QMainWindow *mainWindow){
    mainWindow->close();
    initializeGame();
}
