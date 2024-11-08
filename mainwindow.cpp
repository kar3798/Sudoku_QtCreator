#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudokugame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameWindow(nullptr) // Initializing gameWindow pointer
{
    ui->setupUi(this);

    // Connecting each individual button to corresponding slot
    connect(ui->pushButtonEasy, &QPushButton::clicked, this, &MainWindow::startEasyGame);
    connect(ui->pushButtonMedium, &QPushButton::clicked, this, &MainWindow::startMediumGame);
    connect(ui->pushButtonHard, &QPushButton::clicked, this, &MainWindow::startHardGame);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Factory pattern to start the game based on the difficulty
void MainWindow::startGame(Difficulty difficulty)
{
    // We check if game window already exists and deleting it if needed
    if(gameWindow){
        delete gameWindow;
        gameWindow = nullptr;
    }

    // Create a new GameWindow with the chosen difficulty
    gameWindow = new GameWindow(difficulty, this);
    setCentralWidget(gameWindow);
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


