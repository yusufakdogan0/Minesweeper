#include <QLabel>
#include <iostream>
#include "MineField.h"
#include "Functions.h"



// Constructor
MineField::MineField(int numOfRows, int numOfColumns, int numOfMines, int cellSize)
    : numOfRows(numOfRows), numOfColumns(numOfColumns), numOfMines(numOfMines), score(0), cellSize(cellSize), hint_cell(
        nullptr) {

    // The default icon for our closed cells
    QIcon emptyIcon(R"(:\assets\empty.png)");
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

            // Set the context menu policy to use right click
            curCell->setContextMenuPolicy(Qt::CustomContextMenu);

            // Connect left-click signal to the necessary functions
            connect(curCell, &Cell::clicked, [=]() {
                reveal(row, col); // reveal will reveal current cell and if it is mine it will call function from man
                updateScore(this); // Update the score
                checkWin(this); // Check win
            });

            // Connect right click to onRightClick function
            connect(curCell, &Cell::customContextMenuRequested, [=]() {
                onRightClick(row, col);
            });

            // Add current cell to matrix
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

    // Assign status to cells
    // Status is -1 if the cell is a mine and if not then it will be equal to the number of adjecent mines
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            if( widgetMatrix[row][col]->status != -1){
                widgetMatrix[row][col]->status = countAdjacentMines(row,col);
            }
        }
    }
}


// Counts the number of adjacent mines
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

    // Do not open an already opened cell
    if(curCell->opened){
        return;
    }

    // If it is a mine BOOM
    if(curCell->status == -1){
        loseGame(this);
        return;
    }

    // if its status is 0 (There is no mine around it) open it and all adjacent cells
    else if(curCell->status == 0){
        // increment the score and open the cell
        score++;
        curCell->opened = true;
        // Create a label to make cell un-clickable
        QLabel *label = new QLabel;
        QPixmap pixmap(R"(:\assets\0.png)");
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
        label->setPixmap(pixmap);
        this->addWidget(label,row,col);

        // For each adjacent cell recall this function
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

    // If its status is bigger than 0 then only open this cell
    else{
        // increment the score and open the cell
        score++;
        curCell->opened = true;
        // Create a label to make cell un-clickable
        QLabel *label = new QLabel;
        std::string address = ":/assets/" + std::to_string(curCell->status) + ".png";
        QPixmap pixmap(address.c_str());
        pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
        label->setPixmap(pixmap);
        this->addWidget(label,row,col);
    }

}

void MineField::onRightClick(int row, int col) {
    Cell *curCell = widgetMatrix[row][col];
    if (!curCell->flagged) {
        // Load the flag icon
        QIcon flagIcon(R"(:\assets\flag.png)");

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
        if (curCell == hint_cell){
            hint_cell = nullptr;
        }
    }
    else {
        // If the cell is already flagged, remove the flag icon
        // Set it back to the original empty icon
        QIcon emptyIcon(R"(:\assets\empty.png)");
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

// This function ends the game by making all cells un-clickable
void MineField::endGame() {
    for (int row = 0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfColumns; ++col) {
            // For each cell
            Cell *curCell = widgetMatrix[row][col];

            // If cell is not opened put the necessary label on it
            if (!curCell->opened){
                QLabel *label = new QLabel;

                //If cell is mine
                if (curCell->status == -1){
                    QPixmap pixmap(":/assets/mine.png");
                    pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
                    label->setPixmap(pixmap);
                    this->addWidget(label,row,col);
                }

                // For others
                else{
                    QPixmap pixmap(":/assets/empty.png");
                    pixmap = pixmap.scaled(QSize(cellSize - 1, cellSize - 1), Qt::KeepAspectRatioByExpanding);
                    label->setPixmap(pixmap);
                    this->addWidget(label,row,col);
                }
            }
        }
    }
}

// Returns a vector consists of closed cells used in hint
std::vector<Cell*> MineField::adjacentUnrevealedCells(Cell *cell){
    std::vector<Cell*> return_list;

    // For each adjacent cell if cell is not opened add it to the vector
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; ++i) {
        int newRow = cell->row + dx[i];
        int newCol = cell->column + dy[i];
        if (newRow >= 0 && newRow < this->numOfRows && newCol >= 0 && newCol < this->numOfColumns) {
            Cell *adjacent_cell = widgetMatrix[newRow][newCol];
            if (!adjacent_cell->opened){
                return_list.push_back(adjacent_cell);
            }
        }
    }
    return return_list;
}


// Used to find a safe cell o give hint after virtual flags are put
Cell* MineField::findSafeCell(Cell *cell){
    // Total number of virtual flags in adjacent cells
    int virtual_flag_count = 0;

    // All adjacent cells
    std::vector<Cell*> adjacent_cells;
    adjacent_cells = adjacentUnrevealedCells(cell);

    // If adjacent cell has a virtual flag remove it from the list and update the flag count
    for (int i = adjacent_cells.size() - 1; i >= 0; i--) {
        if(adjacent_cells.at(i)->virtual_flag){
            virtual_flag_count++;
            adjacent_cells.erase(adjacent_cells.begin() + i);
        }
    }

    // If there are other mines or no safe cell return null pointer
    if (adjacent_cells.size() == 0 || cell->status != virtual_flag_count){
        return nullptr;
    }
    // If there are safe cells return the first element of the list
    return adjacent_cells.at(0);
}



