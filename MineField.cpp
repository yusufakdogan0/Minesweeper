#include "MineField.h"

MineField::MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize)
    : numOfRows(numOfRows), numOfColumns(numOfColumns), numOfMines(numOfMines) {

    QIcon emptyIcon(R"(C:\Users\DELL\Desktop\Project3\assets\0.png)");
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
