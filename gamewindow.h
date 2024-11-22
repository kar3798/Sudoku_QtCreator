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
    int previousValue;
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
    void toggleNotesMode(bool enabled); // Slot to toggle notes mode
    void closeGameWindow();             // Slot to close the game window


private:
    Ui::GameWindow *ui;

    SudokuGame *game;                       // Game logic for Sudoku
    QLineEdit *selectedCell;                // Pointer to the currently selected cell
    QTimer *timer;                          // Timer to track the elapsed time
    int elapsedTime;                        // Stores elapsed time in seconds
    int mistakeCount;                       // Counter to track the number of mistakes
    bool isPaused;                          // Boolean for Pause state
    bool notesEnabled;                      // Boolean for notes mode

    QMap<QLineEdit*, QSet<int>> cellNotes;                  // Map to store notes for each cell
    void loadInitialGrid();                                 // Loads the puzzle grid
    void checkAndHandleInput(int row, int col, int number); // Handle input checking
    void setButtonEnabled(bool enabled);                     // To enable and disable buttons when game is paused
    QList<QPushButton*> interactiveButtons;                 // List to store all interactive buttons
    void updateCellDisplay(QLineEdit *cell);                // Method to update cell based on the notes


    QStack<Move> moveStack;                 // Stack called move to store moves for undo functionality

signals:
    void gameWindowClosed();
};

#endif // GAMEWINDOW_H
