#ifndef HIGHSCORESERVICE_H
#define HIGHSCORESERVICE_H

#include <sqlite3.h>

#include "highscoredto.h"

class HighscoreService
{
public:
    HighscoreService();
    ~HighscoreService();
protected:
    const static char* DATABASE_NAME;
    static sqlite3* db;
};

#endif // HIGHSCORESERVICE_H
