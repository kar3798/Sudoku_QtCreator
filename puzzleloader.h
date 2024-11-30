#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QVector>
#include <QString>
#include <QPair>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include "sudokugame.h"

class PuzzleLoader
{
public:
    // Returns a pair of grids: the initial puzzle and solution to the puzzle
    static std::pair<QVector<QVector<int>>, QVector<QVector<int>>> loadPuzzle(Difficulty difficulty);

private:
    static QVector<int> parseRow(const QString &rowString);

};

#endif // PUZZLELOADER_H
