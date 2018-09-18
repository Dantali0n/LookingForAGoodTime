#include "highscoreservice.h"

/**
 * @brief Name that will be chosen for the database
 */
const char* HighscoreService::DATABASE_NAME = "sqlhighscores.db";
sqlite3* HighscoreService::db = nullptr;

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
}

HighscoreService::~HighscoreService()
{
    // Always close the database connection
    fprintf(stderr, "Closing database connection\n");
    sqlite3_close(db);
}
