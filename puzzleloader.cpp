#include "puzzleloader.h"
#include <QDir>

std::pair<QVector<QVector<int>>, QVector<QVector<int>>> PuzzleLoader::loadPuzzle(Difficulty difficulty) {
    QVector<QVector<int>> puzzle(9, QVector<int>(9, 0));
    QVector<QVector<int>> solution(9, QVector<int>(9, 0));

    //Load the JSON file
    QFile jsonFile(":/puzzle/sudoku_puzzles_formatted.json");
    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Failed to open the file";
        return {puzzle, solution};
    }

    // Parse JSON content
    QByteArray jsonFileData = jsonFile.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFileData);

    if(!jsonDoc.isObject()){
        qWarning() <<"Invalid JSON format.";
        return {puzzle, solution};
    }

    QJsonObject jsonObj =jsonDoc.object();
    QString difficultyKey;

    // Determine the key based on difficulty
    switch (difficulty) {
    case EASY: difficultyKey = "EASY"; break;
    case MEDIUM: difficultyKey = "MEDIUM"; break;
    case HARD: difficultyKey = "HARD"; break;
    default: qWarning() << "Unknown difficulty."; return {puzzle, solution};
    }

    // Get puzzles for the specified difficulty
    QJsonArray puzzlesArray = jsonObj[difficultyKey].toArray();
    if (puzzlesArray.isEmpty()) {
        qWarning() << "No puzzles found for difficulty:" << difficultyKey;
        return {puzzle, solution};
    }

    // Randomly select a puzzle
    int randomIndex = QRandomGenerator::global()->bounded(puzzlesArray.size());
    QJsonObject selectedPuzzle = puzzlesArray[randomIndex].toObject();

    // Parse the puzzle grid
    QJsonArray puzzleArray = selectedPuzzle["puzzle"].toArray();
    for (int i = 0; i < 9; ++i) {
        puzzle[i] = parseRow(puzzleArray[i].toString());
    }

    // Parse the solution grid
    QJsonArray solutionArray = selectedPuzzle["solution"].toArray();
    for (int i = 0; i < 9; ++i) {
        solution[i] = parseRow(solutionArray[i].toString());
    }

    return {puzzle, solution};
}

// Helper function to parse a row string into QVector<int>
QVector<int> PuzzleLoader::parseRow(const QString &rowString) {
    QVector<int> parsedRow;
    QStringList rowValues = rowString.split(" ", Qt::SkipEmptyParts);
    for (const QString& value : rowValues) {
        parsedRow.append(value.toInt());
    }
    return parsedRow;
}
//     if (difficulty == EASY) {
//         puzzle = {
//             {5, 3, 0, 0, 7, 0, 0, 0, 0},
//             {6, 0, 0, 1, 9, 5, 0, 0, 0},
//             {0, 9, 8, 0, 0, 0, 0, 6, 0},
//             {8, 0, 0, 0, 6, 0, 0, 0, 3},
//             {4, 0, 0, 8, 0, 3, 0, 0, 1},
//             {7, 0, 0, 0, 2, 0, 0, 0, 6},
//             {0, 6, 0, 0, 0, 0, 2, 8, 0},
//             {0, 0, 0, 4, 1, 9, 0, 0, 5},
//             {0, 0, 0, 0, 8, 0, 0, 7, 9}
//         };
//         solution = {
//             {5, 3, 4, 6, 7, 8, 9, 1, 2},
//             {6, 7, 2, 1, 9, 5, 3, 4, 8},
//             {1, 9, 8, 3, 4, 2, 5, 6, 7},
//             {8, 5, 9, 7, 6, 1, 4, 2, 3},
//             {4, 2, 6, 8, 5, 3, 7, 9, 1},
//             {7, 1, 3, 9, 2, 4, 8, 5, 6},
//             {9, 6, 1, 5, 3, 7, 2, 8, 4},
//             {2, 8, 7, 4, 1, 9, 6, 3, 5},
//             {3, 4, 5, 2, 8, 6, 1, 7, 9}
//         };
//     } else if (difficulty == MEDIUM) {
//         // Add a medium puzzle
//     } else if (difficulty == HARD) {
//         // Add a hard puzzle
//     }

//     return {puzzle, solution};
// }
