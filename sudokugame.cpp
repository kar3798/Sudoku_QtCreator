#include "sudokugame.h"
#include "puzzleloader.h"
#include <QDebug>

SudokuGame::SudokuGame(Difficulty difficulty, QObject *parent)
    : QObject(parent), difficulty(difficulty) {
    // Initialize the grid based on difficulty
    initializeGrid();
}

// void SudokuGame::initializeGrid() {
//     auto [puzzle, solutionGrid] = PuzzleLoader::loadPuzzle(difficulty);

//     // Basic validation logic to check that puzzle and solution are 9x9
//     if (puzzle.size() == 9 && solutionGrid.size() == 9){
//         for (int i = 0; i < 9; ++i) {
//             if (puzzle[i].size() != 9 || solutionGrid[i].size() !=9){
//                 qWarning() << "Invalid puzzle or solution size";
//                 return; // Exit if grid size is invalid
//             }
//         }
//         grid = puzzle;
//         solution = solutionGrid; // Storing solution for checking and hint generation
//     } else {
//         qWarning() << "Invalid puzzle or solution size";
//     }
// }

void SudokuGame::initializeGrid() {
    auto [puzzle, solutionGrid] = PuzzleLoader::loadPuzzle(difficulty);
    grid = puzzle;
    solution = solutionGrid;
}

QVector<QVector<int>> SudokuGame::getGrid() const {
    return grid;
}

bool SudokuGame::isCellEditable(int row, int col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9 ) return false; // Boundry check
    return grid[row][col] == 0;
}

void SudokuGame::setCellValue(int row, int col, int value) {
    if ( row < 0 || row >= 9 || col < 0 || col >= 9) return; // Boundry check
    if (isCellEditable(row, col) && value >= 1 && value <= 9) { // Checks if value is between 1 & 9
        grid[row][col] = value;
    }
}

bool SudokuGame::checkSolution() const {
    // Check rows, columns, and subgrids for uniqueness
    for (int row = 0; row < 9; ++row) {
        QVector<bool> seen(10, false);
        for (int col = 0; col < 9; ++col) {
            int value = grid[row][col];
            if (value == 0 || seen[value]) return false;
            seen[value] = true;
        }
    }

    for (int col = 0; col < 9; ++col) {
        QVector<bool> seen(10, false);
        for (int row = 0; row < 9; ++row) {
            int value = grid[row][col];
            if (value == 0 || seen[value]) return false;
            seen[value] = true;
        }
    }

    for (int startRow = 0; startRow < 9; startRow += 3) {
        for (int startCol = 0; startCol < 9; startCol += 3) {
            QVector<bool> seen(10, false);
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    int value = grid[startRow + row][startCol + col];
                    if (value == 0 || seen[value]) return false;
                    seen[value] = true;
                }
            }
        }
    }

    qDebug() << "Puzzle solved successfully!";
    return true;
}

int SudokuGame::getSolutionValue(int row, int col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) {
        qWarning() << "getSolutionValue: Invalid row/col:" << row << "," << col;
        return -1; // Return -1 or an error code if out of bounds
    }
    qDebug() << "getSolutionValue: Value for (" << row << "," << col << ") is:" << solution[row][col];
    return solution[row][col];
}

