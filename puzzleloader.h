#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include <QVector>
#include "sudokugame.h"

class PuzzleLoader
{
public:

    // Returns a pair of grids, the initial puzzle and solution to the puzzle
    static std::pair<QVector<QVector<int>>, QVector<QVector<int>>> loadPuzzle(Difficulty difficulty);
};

#endif // PUZZLELOADER_H
