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
    HighscoreDTO();
    HighscoreDTO(HighscoreModel hs);
    HighscoreDTO(int id, char* name[], int datetime, uint64_t score);

    HighscoreModel* get();

                                // AFFINITY     -   DATA TYPE
    int id;                     // INTEGER      -   UNSIGNED BIG INT
    char name[64];              // TEXT         -   VARCHAR(64)
    char datetime[19];          // NUMERIC      -   DATETIME
    uint64_t score;             // INTEGER      -   UNSIGNED BIG INT
};

#endif // HIGHSCOREDTO_H
