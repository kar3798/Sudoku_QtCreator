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
    , mistakeCount(0) // Initialize mistake count to zero
    , isPaused(false) // Initial timer is not paused
    , notesEnabled(false) // Initialize notes mode to off

{
    ui->setupUi(this);

    // Disable minimize, maximize, and close buttons
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

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

    // Hint button setup
    connect(ui->toggleNotesMode, &QCheckBox::toggled, this, &GameWindow::toggleNotesMode);

    // Button to close window
    connect(ui->pushButtonCloseWindow, &QPushButton::clicked, this, &GameWindow::closeGameWindow);
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

    if (selectedCell && !selectedCell->isReadOnly()) { // Ensure the cell is editable
        qDebug() << "Updating selected cell:" << selectedCell->objectName();

        // Extract row and column from the selected cell's name
        QString name = selectedCell->objectName();
        int row = name.mid(4, 1).toInt() - 1;
        int col = name.mid(5, 1).toInt() - 1;

        // Get the current value of the cell
        int previousValue = selectedCell->text().isEmpty() ? 0 : selectedCell->text().toInt();

        // Push only the row and column of the move onto the stack
        moveStack.push({row, col});

        if (notesEnabled) {
            // Notes Mode: Replace any existing note with the current number
            cellNotes[selectedCell].clear(); // Clear any existing note
            cellNotes[selectedCell].insert(number); // Add the current note
            updateCellDisplay(selectedCell); // Update the cell display to show the note
        } else {
            // Normal Mode: Add a final answer
            cellNotes[selectedCell].clear(); // Clear all notes for this cell
            updateCellDisplay(selectedCell); // Update the display to remove notes

            // Set the entered number in the selected cell
            selectedCell->setText(QString::number(number));

            // Reset the cell's style for normal mode (e.g., black text)
            selectedCell->setStyleSheet("color: blue; background-color: white; border: 1px solid #31cc02;");

            selectedCell->setAlignment(Qt::AlignCenter); // Center-align the number

            // Update the game grid
            game->setCellValue(row, col, number);

            // Check if the entered number is correct and handle mistakes
            checkAndHandleInput(row, col, number);
        }
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
               // Getting the last value
                if (lastMove.previousValue == 0){
                   cell->clear();
                } else {
                    cell->setText(QString::number(lastMove.previousValue));
                }

                // Reset the cell's style
                cell->setStyleSheet("color: blue; background-color: white; border: 1px solid #31cc02;");
                cell->setAlignment(Qt::AlignCenter);

                // Clear notes for this cell
                cellNotes[cell].clear();
                updateCellDisplay(cell);

                // Update game grid to show the undone move
                game->setCellValue(lastMove.row, lastMove.col, lastMove.previousValue);

                qDebug() << "Undo last move: row" << lastMove.row << ", col" << lastMove.col
                         << ", restored value:" << lastMove.previousValue;
        } else {
            qDebug() << "No moves to undo";
        }
    }
}

void GameWindow::toggleNotesMode(bool enabled){
    notesEnabled = enabled;
}

void GameWindow::setButtonEnabled(bool enabled) {
    for (auto button : interactiveButtons)  {
        button->setEnabled(enabled);
    }
}

void GameWindow::updateCellDisplay(QLineEdit *cell) {
    if (!cellNotes[cell].isEmpty()) {
        // Display the single note in the top-left corner
        int note = *cellNotes[cell].begin(); // Get the single note from the set
        QString notesText = QString::number(note);

        // Set the smaller font for notes
        cell->setFont(QFont("Arial Black", 7)); // Small font for notes
        cell->setText(notesText);
        cell->setAlignment(Qt::AlignTop | Qt::AlignLeft); // Align notes to the top-left corner

        // Set the style for notes mode
        cell->setStyleSheet("color: blue; background-color: white; border: 1px solid #31cc02;");
    } else {
        // Clear the cell if no notes are present
        cell->setFont(QFont("Arial Black", 10)); // Reset to normal font for final answer
        cell->setAlignment(Qt::AlignCenter);

        // Reset the cell's style for normal mode
        cell->setStyleSheet("color: blue; background-color: white; border: 1px solid #31cc02;");
    }
}

void GameWindow::closeGameWindow(){
    // Stop the timer if it's running
    if (timer->isActive()) {
        timer->stop();
    }

    // Close the current game window
    this->close();

    // Emit a signal to notify MainWindow to reload itself
    emit gameWindowClosed();
}
