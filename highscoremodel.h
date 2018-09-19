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
    int id;
    QString name;
    QDateTime datetime;
    uint64_t score;
};

#endif // HIGHSCOREMODEL_H
