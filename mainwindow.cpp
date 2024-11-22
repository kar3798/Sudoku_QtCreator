#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudokugame.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameWindow(nullptr) // Initializing gameWindow pointer
{
    startUpSetup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startUpSetup()
{
    ui->setupUi(this);

    // Connecting each individual button to corresponding slot
    connect(ui->pushButtonEasy, &QPushButton::clicked, this, &MainWindow::startEasyGame);
    connect(ui->pushButtonMedium, &QPushButton::clicked, this, &MainWindow::startMediumGame);
    connect(ui->pushButtonHard, &QPushButton::clicked, this, &MainWindow::startHardGame);
}

// Factory pattern to start the game based on the difficulty
void MainWindow::startGame(Difficulty difficulty)
{
    // We check if game window already exists and deleting it if needed
    if(!gameWindow){
        // Create a new GameWindow with the chosen difficulty
        gameWindow = new GameWindow(difficulty, this);

        // Connect the gameClosed signal to reset the main window
        connect(gameWindow, &GameWindow::gameWindowClosed, this, &MainWindow::reloadMainWindow);

        setCentralWidget(gameWindow);
    }
}

void MainWindow::startEasyGame()
{
    startGame(Difficulty::EASY);
}

void MainWindow::startMediumGame()
{
    startGame(Difficulty::MEDIUM);
}

void MainWindow::startHardGame()
{
    startGame(Difficulty::HARD);
}

void MainWindow::reloadMainWindow()
{
    // Delete the game window
    if (gameWindow) {
        delete gameWindow;
        gameWindow = nullptr;
    }

    // Reset the main window as the central widget
    startUpSetup();
}
