#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startUpSetup();

private slots:
    void startEasyGame();   // Slot fot Easy Button

    void startMediumGame(); // Slot for Medium Button

    void startHardGame();   // Slot for Hard Button

    void reloadMainWindow(); // Reloads main window when game window is closed
private:
    Ui::MainWindow *ui;

    GameWindow *gameWindow; // Pointer to the game window which is responsible for loading the game

    // Factory method to handle game window creation based on the difficulty
    void startGame(Difficulty difficulty);
};
#endif // MAINWINDOW_H
