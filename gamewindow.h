#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "sudokugame.h"
#include <QLineEdit>
#include <QTimer>
#include <QStack>
#include <QPushButton>

//Defining a struct MOVE

struct Move{
    int row;
    int col;
};

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
    void togglePause();                 // Slot that manages the pause logic
    void undoLastMove();                // Slot to undo the last move
    void setButtonEnabled(bool enable); // Slot to enable and disable buttons when game is paused


private:
    Ui::GameWindow *ui;

    SudokuGame *game;                   // Game logic for Sudoku
    QLineEdit *selectedCell;            // Pointer to the currently selected cell
    void loadInitialGrid();
    QTimer *timer;                      // Timer to track the elapsed time
    int elapsedTime;                    // Stores elapsed time in seconds
    bool isPaused;                      // Boolean for Pause state
    void checkAndHandleInput(int row, int col, int number); // Handle input checking
    int mistakeCount;                   // Counter to track the number of mistakes
    QList<QPushButton*> interactiveButtons; // List to store all interactive buttons


    QStack<Move> moveStack;             // Stack called move to store moves for undo functionality
};

#endif // GAMEWINDOW_H
