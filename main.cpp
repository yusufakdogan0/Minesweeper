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
#define numOfRows 20
#define numOfColumns 20
#define numOfMines 40

QLabel *score_label;
bool hint_active = false;
Cell *hint_cell;
bool game_over = false;

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
        game_over = true;
    }
}
void loseGame(MineField *minefield){
    minefield->endGame();
    QMessageBox *messageBox = new QMessageBox;
    messageBox->setText("You Lose!");
    messageBox->show();
    game_over = true;
}
void initializeGame(){
    game_over = false;
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
    QObject::connect(hintButton, &QPushButton::clicked, [=]() {
        hint(mineField);
    });


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

void hint(MineField *minefield){
    if(game_over){
        return;
    }
    if (hint_active){
        minefield->reveal(hint_cell->row, hint_cell->column);
        updateScore(minefield);
        checkWin(minefield);
        hint_active = false;
        return;
    }

    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            Cell *current_cell = minefield->widgetMatrix[row][col];
            if (current_cell->status <= 0 || !current_cell->opened){
                continue;
            }
            std::vector<Cell*> adjacent_cells = minefield->adjacentUnrevealedCells(current_cell);
            if (adjacent_cells.size() == current_cell->status){
                for (int i = 0; i < adjacent_cells.size(); ++i) {
                    adjacent_cells.at(i)->virtual_flag = true;
                }
            }
        }
    }

    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            Cell *current_cell = minefield->widgetMatrix[row][col];
            if (current_cell->status <= 0 || !current_cell->opened){
                continue;
            }
            Cell *safe_cell = minefield->findSafeCell(current_cell);
            if (safe_cell != nullptr){
                QIcon hint_icon(R"(..\assets\hint.png)");
                QPixmap pixmap = hint_icon.pixmap(QSize(cellSize, cellSize));
                // Scale pixmap to fit the button size
                pixmap = pixmap.scaled(QSize(cellSize-1, cellSize-1), Qt::KeepAspectRatioByExpanding);
                QIcon resizedIcon(pixmap);
                safe_cell->setIcon(resizedIcon);

                hint_active = true;
                hint_cell = safe_cell;
                return;
            }
        }
    }

}
