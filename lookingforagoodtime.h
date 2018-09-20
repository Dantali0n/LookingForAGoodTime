#ifndef LOOKINGFORAGOODTIME_H
#define LOOKINGFORAGOODTIME_H

#include <QKeyEvent>
#include <QMainWindow>

#include "ui_lookingforagoodtime.h"

namespace Ui {
class LookingForAGoodTime;
}

class LookingForAGoodTime : public QMainWindow
{
    Q_OBJECT

public:
    explicit LookingForAGoodTime(QWidget* parent = nullptr);
    ~LookingForAGoodTime() override;

private slots:
    void on_exitButton_clicked();
    void on_freePlayButton_clicked();
    void on_pushButton_clicked();
    void on_challengeButton_clicked();

    void on_highscoresButton_clicked();
    void on_backButton_clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent *) override;

private:
    Ui::LookingForAGoodTime *ui;

    QPoint getCentralWidgetFrameCenterPoint();
    QRect getHitbox(QLabel* arm);
    void setClockNumbersGeometry(QPoint center);
    QString getHourString(int time);
    QString getMinuteString(int time);
    void setTimeLabel();
};

#endif // LOOKINGFORAGOODTIME_H

