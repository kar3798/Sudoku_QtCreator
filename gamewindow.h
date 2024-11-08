#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "sudokugame.h"
#include <QLineEdit>
#include <QTimer>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(Difficulty difficulty, QWidget *parent = nullptr);
    ~GameWindow();

private slots:

    void handleNumberInput(int number);
    void selectCell();
    void updateTimer();                 // Slot to update the game timer

    void on_pushButton1_clicked();

private:
    Ui::GameWindow *ui;

    SudokuGame *game;                   // Game logic for Sudoku
    QLineEdit *selectedCell;            // Pointer to the currently selected cell
    void loadInitialGrid();
    QTimer *timer;                      // Timer to track the elapsed time
    int elapsedTime;                    // Stores elapsed time in seconds
    void checkAndHandleInput(int row, int col, int number); // Handle input checking
    int mistakeCount;                   // Counter to track the number of mistakes
};

#endif // GAMEWINDOW_H
