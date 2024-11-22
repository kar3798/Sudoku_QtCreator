#ifndef SUDOKUGENERATOR_H
#define SUDOKUGENERATOR_H

#include <QVector>

class SudokuGenerator
{
public:
    SudokuGenerator();

    // Function to generate the 9x9 grid
    QVector<QVector <int>> generateSudokuGrid();

private:
    // Helper function to fill the grid using backtracking algorith
    bool fillGrid (QVector<QVector<int>> &grid, int row, int col);

    // Helper function for random numbers list
    QVector<int> getRandomNumber();

    // Function to check if the cell (row x column) is safe for a number to be placed
    bool isSafe(const QVector<QVector <int>> &grid, int row, int col, int num);
};

#endif // SUDOKUGENERATOR_H
