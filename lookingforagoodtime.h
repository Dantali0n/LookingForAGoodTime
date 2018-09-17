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
    explicit LookingForAGoodTime(QWidget *parent = nullptr);
    ~LookingForAGoodTime();
protected:
    void keyPressEvent(QKeyEvent *);
private slots:
    void on_exitButton_clicked();

private:
    Ui::LookingForAGoodTime *ui;
};

#endif // LOOKINGFORAGOODTIME_H
