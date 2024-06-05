/**Our code has 2 classes:
 * Cell which inherits button
 * MineField which inherits QGridLayout
 * Cell is represents each square in a minefield
 * Minefield represents collection of all cells
 * We have a window which contains a vertical box
 * This vertical box contains a horizontal box and a minefield
 * Horizontal box has restart, hint buttons and score label
 */

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include "MineField.h"
#include "Cell.h"
#include "Functions.h"

#define cellSize 30 // size of a single cell
#define numOfRows 10 // Total number of rows
#define numOfColumns 10 // Total number of columns
#define numOfMines 9 // Total number of mines


// Global variables which we will need to access globally
QLabel *score_label; // The label which will represent score
bool game_over = false; // it will be true if the game is over

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    initializeGame(); // Opens the game
    return QApplication::exec();
}

//
void initializeGame(){
    game_over = false;
    // Create main window
    QMainWindow *mainWindow = new QMainWindow;

    // Create game setup
    QWidget *mainWidget = new QWidget;
    mainWidget->setFixedHeight(numOfRows * cellSize + 50);
    mainWidget->setFixedWidth(numOfColumns * cellSize + 20);
    QVBoxLayout *mainBox = new QVBoxLayout;
    QHBoxLayout *horizontalBox = new QHBoxLayout;

    // Create score label
    score_label = new QLabel;
    score_label->setText("Score: 0");

    // Create restart button
    QPushButton *restartButton = new QPushButton;
    restartButton->setText("Restart");
    // Create hint button
    QPushButton *hintButton = new QPushButton;
    hintButton->setText("Hint");


    // Add labels and buttons to a horizontal box which will be at top
    horizontalBox->addWidget(score_label);
    horizontalBox->addWidget(restartButton);
    horizontalBox->addWidget(hintButton);
    mainBox->addLayout(horizontalBox);

    // Create minefield and cells
    MineField *mineField = new MineField(numOfRows, numOfColumns, numOfMines, cellSize);
    mineField->setVerticalSpacing(0);
    mineField->setSpacing(0);

    // Handling button functions
    QObject::connect(restartButton, &QPushButton::clicked, [=]() {
        restartGame(mainWindow); // This function will restart the game
    });
    QObject::connect(hintButton, &QPushButton::clicked, [=]() {
        hint(mineField); // This function will give hints
    });

    // Add the container widget to the vertical layout
    mainBox->addLayout(mineField);
    mainWidget->setLayout(mainBox);

    // Handle the window
    mainWindow->setCentralWidget(mainWidget);
    mainWindow->setWindowTitle("Minesweeper");
    QIcon icon(R"(:\assets\mine.png)");
    mainWindow->setWindowIcon(icon);
    mainWindow->show();
}

// Updates the score
void updateScore(MineField *minefield){
    QString text = "Score: " + QString::number(minefield->score);
    score_label->setText(text);
}

// Closes the game for glorious winners
void checkWin(MineField *minefield){
    // score + mines == total number of cells is win condition
    if(minefield->score + numOfMines == numOfColumns * numOfRows){
        // first we need to end the game and make minefield un-clickable
        minefield->endGame();

        // Then we need to pop up a message box to inform player
        QMessageBox *messageBox = new QMessageBox;
        messageBox->setText("You Win!");
        messageBox->show();

        // End the game in main
        game_over = true;
    }
}

// Closes the game for loser noobs
void loseGame(MineField *minefield){
    // first we need to make sure our player does not step on another mine
    minefield->endGame();

    // Then we need to remind him his status
    QMessageBox *messageBox = new QMessageBox;
    messageBox->setText("You Lose!");
    messageBox->show();

    // End the game in main
    game_over = true;
}

// Closes the current game and opens a new one
void restartGame(QMainWindow *mainWindow){
    mainWindow->close();
    initializeGame();
}


// Hint algorithm first puts virtual flags to the points we can be sure there exist a mine
// Then opens one of remaining cells using this information
void hint(MineField *minefield){
    // No need to give hint if game is over
    if(game_over){
        return;
    }

    // If hint is already given open the cell
    if (minefield->hint_cell != nullptr && !minefield->hint_cell->opened){
        // Reveal the cell and update the score
        minefield->reveal(minefield->hint_cell->row, minefield->hint_cell->column);
        updateScore(minefield);
        // Check whether win condition is satisfied or not
        checkWin(minefield);
        return;
    }

    // Add virtual flags
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            // For each cell
            Cell *current_cell = minefield->widgetMatrix[row][col];
            // If cell is not opened find another cell
            if (current_cell->status <= 0 || !current_cell->opened){
                continue;
            }

            // Take the closed adjacent cells
            std::vector<Cell*> adjacent_cells = minefield->adjacentUnrevealedCells(current_cell);

            // If status of a cell equals to the number of adjacent cells then all the adjacent cells are mines
            if ((int) adjacent_cells.size() == current_cell->status){
                for (int i = 0; i < (int) adjacent_cells.size(); ++i) {
                    // Put the virtual flag on them
                    adjacent_cells.at(i)->virtual_flag = true;
                }
            }
        }
    }

    // Now we have to look each opened cell to confirm whether we found all the mines around it or not
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            // For each cell
            Cell *current_cell = minefield->widgetMatrix[row][col];

            // If the cell closed find another cell
            if (current_cell->status <= 0 || !current_cell->opened){
                continue;
            }

            // Find a safe cell using virtual flags
            // this function will return nullptr if there is no safe cell
            Cell *safe_cell = minefield->findSafeCell(current_cell);

            // If there is a safe cell
            if (safe_cell != nullptr){
                // Create an icon for representing hint
                QIcon hint_icon(R"(:\assets\hint.png)");
                QPixmap pixmap = hint_icon.pixmap(QSize(cellSize, cellSize));

                // Scale pixmap to fit the button size
                pixmap = pixmap.scaled(QSize(cellSize-1, cellSize-1), Qt::KeepAspectRatioByExpanding);
                QIcon resizedIcon(pixmap);

                // Change the icon
                safe_cell->setIcon(resizedIcon);

                // Add it as hint_cell so that second usage of hint button will open it
                minefield->hint_cell = safe_cell;
                minefield->hint_cell->flagged = false;
                return;
            }
        }
    }

}
