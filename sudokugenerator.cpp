#include "sudokugenerator.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>


// Constructor initializes the random seed
SudokuGenerator::SudokuGenerator(){

    std::srand(std::time(0)); // Seed for random number generation
}

// Function to create a fully solved 9x9 grid
QVector<QVector<int>> SudokuGenerator::generateSudokuGrid() {
    QVector<QVector<int>> grid(9, QVector<int>(9,0)); // Initializing a grid
    fillGrid(grid, 0, 0); // Filling the grid with 0's
    return grid;
}


// Recursive backtracking function to fill the grid
bool SudokuGenerator::fillGrid(QVector<QVector<int> > &grid, int row, int col) {
    // Base Case: If all rows are filled the grid is complete
    if (row == 9) return true;

    // Move to the next row if col index exceeds 8
    int nextRow = (col == 8) ? col + 1 : col;
    int nextCol = (col + 1) % 9;

    // Skipping the cells that are already filled
    if (grid[row][col] != 0){
        return fillGrid(grid, nextRow, nextCol);
    }

    // Get a random list of numbers to try for the current cell
    QVector<int> numbers = getRandomNumber();
    for (int num : numbers) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num; // Place the number in the cell
            if (fillGrid(grid, nextRow, nextCol)) { // Recur for the next cell
                return true;
            }
            grid[row][col] = 0; // Backtrack and reset the cell
        }
    }

    return false; // No valid number could be placed
}

// Generate the random list of numbers to fill the cells
QVector<int> SudokuGenerator::getRandomNumber() {
    QVector<int> numbers;
    for (int i = 1; i <= 9; ++i) {
        numbers.push_back(i);
    }
    std::random_shuffle(numbers.begin(), numbers.end()); // Shuffle numbers randomly
    return numbers;
}

// Check if is safe to place the number in the given row, col, and subgrid
bool SudokuGenerator::isSafe(const QVector<QVector<int> > &grid, int row, int col, int num) {
    // Check if the number exists in the current row
    for (int x = 0; x < 9; ++x){
        if (grid[row][col] == num)
            return false;
    }

    // Check if the number exists in the current col
    for (int y = 0; y < 9; ++y){
        if(grid[y][col] == num)
            return false;
    }

    // Check if the number exists in thr 3x3 grid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c){
            if (grid[startRow + r][startCol + c] == num)
                return false;
        }
    }

    return true; // Number is safe to place
}

