#include "highscoreservice.h"

/**
 * @brief Name that will be chosen for the database
 */
const char* HighscoreService::DATABASE_NAME = "sqlhighscores.db";

const char* HighscoreService::CREATE_DATABSE_IF_NOT_EXIST =   "CREATE TABLE IF NOT EXISTS HIGHSCORE("  \
                                                    "id UNSIGNED BIG INT PRIMARY KEY    NOT NULL," \
                                                    "name       VARHCAR(64)             NOT NULL," \
                                                    "datetime   DATETIME                NOT NULL," \
                                                    "score      UNSIGNED BIG INT);";

sqlite3* HighscoreService::db = nullptr;
uint32_t HighscoreService::ref = 0;

int HighscoreService::callback(void* unused, int argc, char* argv[], char* colNames[]) {
    return 0;
}

HighscoreService::HighscoreService()
{
    // If no active database connection exists, create it.
    if(db == nullptr)
    {
        // Create database connection and test if the connection succeeded
        if(sqlite3_open(DATABASE_NAME, &db))
        {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            fprintf(stderr, "Opened database successfully\n");
        }
    }
    else {
        fprintf(stderr, "Already have database connection :)\n");
    }

    // increment references count to keep track of active HighscoreService isntances
    ref++;

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, HighscoreService::CREATE_DATABSE_IF_NOT_EXIST, callback, nullptr, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }
}

HighscoreService::~HighscoreService()
{
    // decrement references & close connection if last references
    ref--;
    if(ref == 0)
    {
        fprintf(stderr, "Closing database connection\n");
        sqlite3_close(db);
    }
}
