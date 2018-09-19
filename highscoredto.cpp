#include "highscoredto.h"

const char* HighscoreDTO::DATE_FORMAT = "yyyy-MM-dd HH:mm:ss";

HighscoreDTO::HighscoreDTO() {

}

HighscoreModel* HighscoreDTO::get() {
    HighscoreModel* hs = new HighscoreModel();
    hs->id = id;
    hs->name = name;
    hs->datetime = QDateTime::fromString(datetime, DATE_FORMAT);
    hs->score = score;
    return hs;
}
