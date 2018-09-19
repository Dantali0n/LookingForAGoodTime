#ifndef HIGHSCORESERVICE_H
#define HIGHSCORESERVICE_H

#include <sqlite3.h>
#include <bsd/string.h>
#include <vector>

#include "highscoredto.h"

/**
 * @brief Manages database interactions
 */
class HighscoreService
{
public:
    HighscoreService();
    ~HighscoreService();

    std::vector<HighscoreModel*> get();
    HighscoreModel* get(uint64_t id);

    void put(std::vector<HighscoreModel> hs);
    void put(HighscoreModel hs);

    void patch(uint64_t id, HighscoreModel hs);

    void remove(uint64_t id);
protected:
    const static char* DATABASE_NAME;
    static sqlite3* db;
    static uint32_t ref;
    static int callback(void* unused, int argc, char* argv[], char* colNames[]);

    // Database queries
    const static char* CREATE_DATABSE_IF_NOT_EXIST;
    const static char* GET_ALL;
    const static char* GET_ID;
    const static char* PUT;
};

#endif // HIGHSCORESERVICE_H