#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QLineEdit>
#include <QMouseEvent>

class SudokuCell : public QLineEdit {
    Q_OBJECT

public:
    explicit SudokuCell(QWidget *parent = nullptr) : QLineEdit(parent) {}

signals:
    void cellClicked(); // Signal emitted when the cell is clicked

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit cellClicked();
        QLineEdit::mousePressEvent(event); // Call the base class implementation
    }
};

#endif // SUDOKUCELL_H
