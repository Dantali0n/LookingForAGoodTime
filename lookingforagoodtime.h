#ifndef LOOKINGFORAGOODTIME_H
#define LOOKINGFORAGOODTIME_H

#include <QMainWindow>
#include <QKeyEvent>

namespace Ui {
class LookingForAGoodTime;
}

class LookingForAGoodTime : public QMainWindow
{
    Q_OBJECT

public:
    explicit LookingForAGoodTime(QWidget* parent = nullptr);
    ~LookingForAGoodTime();
protected:
    void keyPressEvent(QKeyEvent *);
private slots:
    void on_exitButton_clicked();

    void on_freePlayButton_clicked();

    void on_pushButton_clicked();

    void on_highscoresButton_clicked();

    void on_backButton_clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void onMouseEvent(const QString &eventName, const QPoint &pos);

private:
    Ui::LookingForAGoodTime *ui;
};

#endif // LOOKINGFORAGOODTIME_H
