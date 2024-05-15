#include <QLabel>
#include <iostream>
#include "MineField.h"
#include "Functions.h"




MineField::MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize)
    : numOfRows(numOfRows), numOfColumns(numOfColumns), numOfMines(numOfMines), score(0), cellSize(cellSize) {

    QIcon emptyIcon(R"(C:\Users\DELL\Desktop\Project3\assets\empty.png)");
    QPixmap pixmap = emptyIcon.pixmap(QSize(cellSize, cellSize));
    // Scale pixmap to fit the button size
    pixmap = pixmap.scaled(QSize(cellSize-1, cellSize-1), Qt::KeepAspectRatioByExpanding);
    QIcon resizedIcon(pixmap);

    // Create Cells
    widgetMatrix.resize(numOfRows);
    for (int row = 0; row < numOfRows; ++row) {
        widgetMatrix[row].resize(numOfColumns);
        for (int col = 0; col < numOfColumns; ++col) {
            Cell *curCell = new Cell(cellSize, row, col);
            curCell->setIcon(resizedIcon); // Set the resized icon
            curCell->setIconSize(QSize(cellSize, cellSize)); // Set the icon size to match the button size
            curCell->setStyleSheet("border: 1px solid black;");


            // Set the context menu policy to Qt::CustomContextMenu
            curCell->setContextMenuPolicy(Qt::CustomContextMenu);

            // Connect left-click signal to the onLeftClick function
            connect(curCell, &Cell::clicked, [=]() {
                reveal(row, col);
                updateScore(this);
                checkWin(this);
            });
            connect(curCell, &Cell::customContextMenuRequested, [=]() {
                onRightClick(row, col);
            });


            widgetMatrix[row][col] = curCell;
            this->addWidget(curCell, row, col);
        }
    }



    // Create Mines
    int count = 0;
    while (count < numOfMines) {
        int i = rand() % numOfRows; // Random row index
        int j = rand() % numOfColumns; // Random column index
        if (widgetMatrix[i][j]->status == 0) {
            widgetMatrix[i][j]->status = -1;
            count++;
        }
    }

    // Assign numbers to cells
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            if( widgetMatrix[row][col]->status != -1){
                widgetMatrix[row][col]->status = countAdjacentMines(row,col);
            }
        }
    }
}



int MineField::countAdjacentMines(int row, int col) {
    int count = 0;
    // Define the eight possible directions
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Check all eight adjacent cells
    for (int i = 0; i < 8; ++i) {
        int newRow = row + dx[i];
        int newCol = col + dy[i];
        // Check if the new coordinates are within bounds
        if (newRow >= 0 && newRow < this->numOfRows && newCol >= 0 && newCol < this->numOfColumns) {
            // If the adjacent cell has a 1, increment the count
            if (widgetMatrix[newRow][newCol]->status == -1) {
                count++;
            }
        }
    }

    return count;
}


// Define your left-click function
void MineField::reveal(int row, int col) {
    Cell *curCell = widgetMatrix[row][col];
    if(curCell->opened ){
        return;
    }
    if (curCell->flagged){
        return;
    }
    // If it is a mine BOOM
    if(curCell->status == -1){
        loseGame(this);
        return;
    }
    // if its status is 0 (There is no mine around it)
    else if(curCell->status == 0){
        score++;
        curCell->opened = true;
        QLabel *label = new QLabel;
        QPixmap pixmap(R"(C:\Users\DELL\Desktop\Project3\assets\0.png)");
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
        label->setPixmap(pixmap);
        this->addWidget(label,row,col);
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int i = 0; i < 8; ++i) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];
            if (newRow >= 0 && newRow < this->numOfRows && newCol >= 0 && newCol < this->numOfColumns) {
                reveal(newRow,newCol);
            }
        }

        return;
    }
    else{
        score++;
        curCell->opened = true;
        QLabel *label = new QLabel;
        std::string address = "C:/Users/DELL/Desktop/Project3/assets/" + std::to_string(curCell->status) + ".png";
        QPixmap pixmap(address.c_str());
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
        label->setPixmap(pixmap);
        this->addWidget(label,row,col);
    }

}

void MineField::onRightClick(int row, int col) {
    Cell *curCell = widgetMatrix[row][col];
    if (!curCell->flagged) {
        // Load the flag icon (assuming you have it)
        QIcon flagIcon(R"(C:\Users\DELL\Desktop\Project3\assets\flag.png)");

        // Create a pixmap from the flag icon
        QPixmap pixmap = flagIcon.pixmap(QSize(cellSize, cellSize));

        // Scale pixmap to fit the button size
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);

        // Create a new icon from the scaled pixmap
        QIcon resizedIcon(pixmap);

        // Set the icon to the resized flag icon
        curCell->setIcon(resizedIcon);

        // Set the icon size to match the button size
        curCell->setIconSize(QSize(cellSize, cellSize));

        // Update the flagged state of the cell
        curCell->flagged = true;
    } else {
        // If the cell is already flagged, remove the flag icon
        // You can set it back to the original empty icon or any other desired icon
        QIcon emptyIcon(R"(C:\Users\DELL\Desktop\Project3\assets\empty.png)");
        // Create a pixmap from the flag icon
        QPixmap pixmap = emptyIcon.pixmap(QSize(cellSize, cellSize));

        // Scale pixmap to fit the button size
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);

        // Create a new icon from the scaled pixmap
        QIcon resizedIcon(pixmap);
        curCell->setIcon(resizedIcon);

        // Set the icon size to match the button size
        curCell->setIconSize(QSize(cellSize, cellSize));

        // Update the flagged state of the cell
        curCell->flagged = false;
    }
}

void MineField::endGame() {
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            Cell *curCell = widgetMatrix[row][col];
            if (!curCell->opened){
                QLabel *label = new QLabel;

                if (curCell->status == -1){
                    QPixmap pixmap("C:/Users/DELL/Desktop/Project3/assets/mine.png");
                    pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
                    label->setPixmap(pixmap);
                    this->addWidget(label,row,col);
                }
                else{
                    QPixmap pixmap("C:/Users/DELL/Desktop/Project3/assets/empty.png");
                    pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
                    label->setPixmap(pixmap);
                    this->addWidget(label,row,col);
                }
            }
        }
    }
}



