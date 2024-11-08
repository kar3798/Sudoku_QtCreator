#ifndef SUDOKUGAME_H
#define SUDOKUGAME_H

#include <QObject>
#include <QVector>

enum Difficulty { EASY, MEDIUM, HARD};

class SudokuGame : public QObject {
    Q_OBJECT

public:
    explicit SudokuGame(Difficulty difficulty, QObject *parent = nullptr);

    // Getter for the Sudoku grid
    QVector<QVector<int>> getGrid() const;

    // Checks if a cell at (row, col) is editable
    bool isCellEditable(int row, int col) const;

    // Sets a value in a specific cell if editable
    void setCellValue(int row, int col, int value);

    // Checks if the solution is correct (basic placeholder logic)
    bool checkSolution() const;

    // Method to retrieve the correct solution value at a given row and column
    int getSolutionValue(int row, int col) const;

private:
    // Initializes the grid based on the difficulty level
    void initializeGrid();

    Difficulty difficulty;
    QVector<QVector<int>> grid; // 9x9 matrix representing the Sudoku grid
    QVector<QVector<int>> solution; // Placeholder for the solution grid, can be used for checking correctness

};

#endif // SUDOKUGAME_H
