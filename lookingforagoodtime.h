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
    explicit LookingForAGoodTime(QWidget* parent = nullptr);
    ~LookingForAGoodTime();

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
