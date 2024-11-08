#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QLineEdit>
#include <QMouseEvent>

class SudokuCell : public QLineEdit {
    Q_OBJECT

public:
    explicit SudokuCell(QWidget *parent = nullptr) : QLineEdit(parent) {}

signals:
    void cellClicked(SudokuCell *cell);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!isReadOnly()) { // Only emit if cell is editable
            emit cellClicked(this);
        }
        QLineEdit::mousePressEvent(event); // Call base class implementation
    }
};

#endif // SUDOKUCELL_H
