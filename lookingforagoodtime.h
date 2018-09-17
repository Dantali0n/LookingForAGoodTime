#ifndef LOOKINGFORAGOODTIME_H
#define LOOKINGFORAGOODTIME_H

#include <QMainWindow>

namespace Ui {
class LookingForAGoodTime;
}

class LookingForAGoodTime : public QMainWindow
{
    Q_OBJECT

public:
    explicit LookingForAGoodTime(QWidget *parent = nullptr);
    ~LookingForAGoodTime();

private:
    Ui::LookingForAGoodTime *ui;
};

#endif // LOOKINGFORAGOODTIME_H
