#ifndef HIGHSCOREMODEL_H
#define HIGHSCOREMODEL_H

#include <QDateTime>
#include <QString>

/**
 * @brief datamodel for highscore data
 */
class HighscoreModel
{
public:
    uint64_t id;
    QDateTime datetime;
    QString name;
    uint64_t score;
};

#endif // HIGHSCOREMODEL_H
