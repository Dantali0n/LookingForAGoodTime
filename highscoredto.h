#ifndef HIGHSCOREDTO_H
#define HIGHSCOREDTO_H

#include <QDateTime>
#include <QString>

#include "highscoremodel.h"

/**
 * @brief highlevel dto to store and load highscore data.
 */
class HighscoreDTO
{
public:
    HighscoreDTO(HighscoreModel hs);
protected:          // AFFINITY     -   DATA TYPE
    uint64_t id;    // INTEGER      -   UNSIGNED BIG INT
    int datetime;   // NUMERIC      -   DATETIME
    char* name[64]; // TEXT         -   VARCHAR(64)
    uint64_t score; // INTEGER      -   UNSIGNED BIG INT
};

#endif // HIGHSCOREDTO_H
