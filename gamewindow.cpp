#include "gamewindow.h"
#include "sudokugame.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QTime>
#include "sudokucell.h"

// Constructor with Difficulty parameter
GameWindow::GameWindow(Difficulty difficulty, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
    , game(new SudokuGame(difficulty, this))
    , selectedCell(nullptr) // Initialize selectedCell to nullptr
    , elapsedTime(0) // Initialize the time to zero
    , isPaused(false) // Initial timer is not paused
    , mistakeCount(0) // Initialize mistake count to zero

{
    ui->setupUi(this);
    loadInitialGrid();

    // Initializing the timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateTimer);

    // Start the timer with a 1-second interval
    timer->start(1000);

    // Set the initial mistake count in the UI
    ui->mistakeCntLabel->setText("Mistakes: 0/3");
    connect(ui->pushButton1, &QPushButton::clicked, this, [=]() { handleNumberInput(1); });
    connect(ui->pushButton2, &QPushButton::clicked, this, [=]() { handleNumberInput(2); });
    connect(ui->pushButton3, &QPushButton::clicked, this, [=]() { handleNumberInput(3); });
    connect(ui->pushButton4, &QPushButton::clicked, this, [=]() { handleNumberInput(4); });
    connect(ui->pushButton5, &QPushButton::clicked, this, [=]() { handleNumberInput(5); });
    connect(ui->pushButton6, &QPushButton::clicked, this, [=]() { handleNumberInput(6); });
    connect(ui->pushButton7, &QPushButton::clicked, this, [=]() { handleNumberInput(7); });
    connect(ui->pushButton8, &QPushButton::clicked, this, [=]() { handleNumberInput(8); });
    connect(ui->pushButton9, &QPushButton::clicked, this, [=]() { handleNumberInput(9); });

    // Pause button setup
    connect(ui->pushButtonPause, &QPushButton::clicked, this, &GameWindow::togglePause);

    // Undo button setup
    connect(ui->pushButtonUndo, &QPushButton::clicked, this, &GameWindow::undoLastMove);

    ui->pushButtonPause->setText("PAUSE");

    // Initialize the list of buttons to control
    interactiveButtons = {
        ui->pushButton1, ui->pushButton2, ui->pushButton3,
        ui->pushButton4, ui->pushButton5, ui->pushButton6,
        ui->pushButton7, ui->pushButton8, ui->pushButton9,
        ui->pushButtonUndo, ui->pushButtonHint
    };
}

GameWindow::~GameWindow()
{
    delete game;
    delete timer;
}

void GameWindow::loadInitialGrid() {
    auto grid = game->getGrid(); // Get the loaded puzzle grid
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int value = grid[row][col];
            QString cellName = QString("cell%1%2").arg(row + 1).arg(col + 1);
            SudokuCell *cell = findChild<SudokuCell *>(cellName);
            if (cell) {
                // Set text alignment to center
                cell->setAlignment(Qt::AlignCenter);
                if (value != 0) {
                    // Set the initial puzzle value and make it read-only
                    cell->setText(QString::number(value));
                    cell->setReadOnly(true);
                    cell->setStyleSheet("background-color: white; color: black; font-weight: bold; border: 1px solid #ff080c;"); // Style for non-editable cells
                } else {
                    // Make the cell editable and clear any initial text
                    cell->clear();
                    cell->setReadOnly(false);
                    cell->setStyleSheet("background-color: white; color: blue; border: 1px solid #31cc02;"); // Style for editable cells

                    // Connect the cell's click event to selectCell
                    connect(cell, &SudokuCell::cellClicked, this, &GameWindow::selectCell);
                }
            }
        }
    }
}

void GameWindow::checkAndHandleInput(int row, int col, int number) {
    int correctValue = game->getSolutionValue(row, col);
    qDebug() << "Checking cell (" << row << "," << col << ") with entered number:" << number << ", correct value is:" << correctValue;

    if (correctValue == -1) {
        qWarning() << "Invalid cell position!";
        return; // Exit if invalid position
    }

    if (correctValue == number) {
        game->setCellValue(row, col, number);

        // Check if the puzzle is solved
        if (game->checkSolution()) {
            timer->stop(); // Stop the timer
            QMessageBox::information(this, "Congratulations!", "You solved the puzzle!");
            return;
        }
    } else {
        // Increment mistake count
        mistakeCount++;
        qDebug() << "Mistake made! Mistake count:" << mistakeCount;

        // Update UI
        ui->mistakeCntLabel->setText(QString("Mistakes: %1/3").arg(mistakeCount));

        if (mistakeCount >= 3) {
            timer->stop(); // Stop the timer
            QMessageBox::information(this, "Game Over", "You made 3 mistakes. Game Over!");
            close(); // End the game
        }
    }
}

void GameWindow::selectCell() {
    SudokuCell *cell = qobject_cast<SudokuCell *>(sender());
    if (cell && !cell->isReadOnly()) { // Only select editable cells
        selectedCell = cell;
        qDebug() << "Editable cell selected:" << selectedCell->objectName();
    } else {
        qDebug() << "Cell is read-only or selection failed";
    }
}

// Slot to handle the pausing and resuming
void GameWindow::togglePause() {
    if(isPaused){
        // Resume the timer and re-enabling buttons
        timer->start(1000);
        ui->pushButtonPause->setText("PAUSE");
        isPaused = false;

        // Re-enabling the buttons
        setButtonEnabled(true);
    }
    else {
        // Pause the timer and disabling buttons
        timer->stop();
        ui->pushButtonPause->setText("RESUME");
        isPaused = true;

        // Disabling the buttons
        setButtonEnabled(false);
    }
}

// Slot to update the timer display
void GameWindow::updateTimer() {
    if(!isPaused){
        elapsedTime++; // Incrementing elasped time by 1 sec

        // Formatting the elasped time (MM:SS)
        int minutes = elapsedTime / 60;
        int seconds = elapsedTime % 60;

        QString timeText = QString("%1:%2")
                               .arg(minutes, 2, 10, QChar('0'))
                               .arg(seconds, 2, 10, QChar('0'));

        // Update the QLabel on the UI
        ui->timeLabel->setText(timeText);
    }
}

void GameWindow::handleNumberInput(int number) {
    qDebug() << "handleNumberInput called with number:" << number;

    if (selectedCell && !selectedCell->isReadOnly()) { // Check if a cell is selected and editable
        qDebug() << "Updating selected cell:" << selectedCell->objectName();

        // Extract row and column from selected cell's name (e.g., "cell11" for row 1, col 1)
        QString name = selectedCell->objectName();
        int row = name.mid(4, 1).toInt() - 1;
        int col = name.mid(5, 1).toInt() - 1;

        // Push only the row and column of the move onto the stack
        moveStack.push({row, col});

        // Setting the entered number in the selected cell
        selectedCell->setText(QString::number(number));

        // Check if the entered number is correct and handle mistakes
        checkAndHandleInput(row, col, number);
    } else {
        qDebug() << "No selected cell or cell is read-only";
    }
}

void GameWindow::undoLastMove(){
    if(!moveStack.isEmpty()){
        Move lastMove = moveStack.pop(); // Get the last move

        // Finding the corresponding cell based on row and col
        QString cellName = QString("cell%1%2").arg(lastMove.row + 1).arg(lastMove.col + 1);
        QLineEdit *cell = findChild<QLineEdit *>(cellName);

        if (cell && !cell->isReadOnly()) { // Only modify editable cells
            cell->clear(); // Clear the cell if the previous value was empty
            qDebug() << "Undo last move: row" << lastMove.row << ", col" << lastMove.col;
        }
    } else {
        qDebug() << "No moves to undo";
    }
}

void GameWindow::setButtonEnabled(bool enable) {
    for (auto button : interactiveButtons)  {
        button->setEnabled(enable);
    }
}

