#include "highscoreservice.h"

/**
 * @brief Name that will be chosen for the database
 */
const char* HighscoreService::DATABASE_NAME = "sqlhighscores.db";

const char* HighscoreService::CREATE_DATABSE_IF_NOT_EXIST =   "CREATE TABLE IF NOT EXISTS HIGHSCORE("  \
                                                    "id INTEGER PRIMARY KEY AUTOINCREMENT," \
                                                    "name       VARHCAR(64)             NOT NULL," \
                                                    "datetime   DATETIME                NOT NULL," \
                                                    "score      UNSIGNED BIG INT        NOT NULL);";
const char* HighscoreService::GET_ALL_BY_SCORE_LIMIT_10 = "SELECT * FROM HIGHSCORE ORDER BY score DESC LIMIT 10;";
const char* HighscoreService::GET_ALL                   = "SELECT * FROM HIGHSCORE;";
const char* HighscoreService::GET_ID                    = "SELECT * FROM HIGHSCORE WHERE id = ?;";
const char* HighscoreService::PUT                       = "INSERT INTO HIGHSCORE (name, datetime, score) VALUES (?,?,?);";
const char* HighscoreService::DELETE                    = "DELETE FROM HIGHSCORE WHERE id = ?;";

sqlite3* HighscoreService::db = nullptr;
uint32_t HighscoreService::ref = 0;

/**
 * @brief Mandatory but otherwise unused callback.
 */
int HighscoreService::callback(void* unused, int argc, char* argv[], char* colNames[])
{
    int i;
    fprintf(stderr, "%s: ", (const char*)unused);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", colNames[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
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

        // Upon first connection attempt to create required tables if they do not exist.
        char* zErrMsg = nullptr;
        int rc = sqlite3_exec(db, HighscoreService::CREATE_DATABSE_IF_NOT_EXIST, callback, nullptr, &zErrMsg);
        if( rc != SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", zErrMsg);
           sqlite3_free(zErrMsg);
        }
    }
    else {
        fprintf(stderr, "Already have database connection :)\n");
    }

    // increment references count to keep track of active HighscoreService isntances
    ref++;

    /*char* sql = "INSERT INTO HIGHSCORE(name, datetime, score) "  \
            "VALUES ('Paul', '1993-12-12 22:12:00', 20000); ";
    rc = sqlite3_exec(db,  sql, callback, nullptr, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }*/
}

HighscoreService::~HighscoreService()
{
    // decrement references & close connection if last references
    ref--;
    if(ref == 0)
    {
        fprintf(stderr, "Closing database connection\n");
        sqlite3_close(db);
        db = nullptr;
    }
    else {
        fprintf(stderr, "Not the last reference not closing database connection\n");
    }
}

std::vector<HighscoreModel*> HighscoreService::getTop10() {
    std::vector<HighscoreModel*> data;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, HighscoreService::GET_ALL_BY_SCORE_LIMIT_10, -1, &stmt, nullptr);
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        HighscoreDTO dto;
        dto.id = sqlite3_column_int(stmt, 0);
        strncpy(dto.name, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), 60);
        strncpy(dto.datetime, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), 19);
        dto.score = static_cast<unsigned long>(sqlite3_column_int64(stmt, 3));
        HighscoreModel* highscore = dto.get();
        data.push_back(highscore);
    }

    sqlite3_finalize(stmt);
    return data;
}

std::vector<HighscoreModel*> HighscoreService::get()
{
    std::vector<HighscoreModel*> data;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, HighscoreService::GET_ALL, -1, &stmt, nullptr);
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        HighscoreDTO dto;
        dto.id = sqlite3_column_int(stmt, 0);
        strncpy(dto.name, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), 60);
        strncpy(dto.datetime, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), 19);
        dto.score = static_cast<unsigned long>(sqlite3_column_int64(stmt, 3));
        HighscoreModel* highscore = dto.get();
        data.push_back(highscore);
    }

    sqlite3_finalize(stmt);
    return data;
}

HighscoreModel* HighscoreService::get(int id)
{
    HighscoreModel* highscore = nullptr;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, HighscoreService::GET_ID, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        HighscoreDTO dto;
        dto.id = sqlite3_column_int(stmt, 0);
        strncpy(dto.name, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)), 60);
        strncpy(dto.datetime, reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)), 19);
        dto.score = static_cast<unsigned long>(sqlite3_column_int64(stmt, 3));
        highscore = dto.get();
    }
    sqlite3_finalize(stmt);
    return highscore;
}

void HighscoreService::put(std::vector<HighscoreModel> hs)
{
    fprintf(stderr, "UNIMPLEMENTED FUNCTION!");
}

void HighscoreService::put(HighscoreModel hs)
{
    HighscoreDTO ds = HighscoreDTO(hs);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, HighscoreService::PUT, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ds.name, 64, nullptr);
    sqlite3_bind_text(stmt, 2, ds.datetime, 19, nullptr);
    sqlite3_bind_int64(stmt, 3, static_cast<sqlite_int64>(ds.score));
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}

void HighscoreService::patch(int id, HighscoreModel hs)
{
    fprintf(stderr, "UNIMPLEMENTED FUNCTION!");
}

void HighscoreService::remove(int id)
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, HighscoreService::DELETE, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}
