#include "highscoredto.h"

HighscoreDTO::HighscoreDTO() {

}

HighscoreDTO::HighscoreDTO(HighscoreModel hs) {
    id = hs.id;
    strncpy(name, hs.name.toStdString().c_str(), 64);
    strncpy(datetime, hs.datetime.toString().toStdString().c_str(), 19);
    score = hs.score;
}

HighscoreModel* HighscoreDTO::get() {
    HighscoreModel* hs = new HighscoreModel();
    hs->id = id;
    hs->name = name;
    hs->datetime = QDateTime::fromString(datetime, HighscoreModel::DATE_FORMAT);
    hs->score = score;
    return hs;
}
