#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"
#include <QMouseEvent>
#include <QPainter>
#include <math.h>

QLabel* labels[12];

bool isDragging;
QLabel* currentLabel;

LookingForAGoodTime::LookingForAGoodTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LookingForAGoodTime)
{
    ui->setupUi(this);

    QRect mainRect = ui->centralWidget->geometry();
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();

    const double PI = 3.1415926535;
    int x = mainWidth * 4;
    int y = mainHeight * 4;
    int r = 100;

    double angle;
    double x1;
    double y1;

    for (int i = 0; i < 12; i++) {
        angle = (i + 1 + 9) * 360 / 12;
        x1 = r * cos(angle * PI / 180);
        y1 = r * sin(angle * PI / 180);

        QLabel *label = new QLabel(this);
        label->setObjectName("label_" + QString::number(i + 1));
        label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        label->setText(QString::number(i + 1));
        label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
        label->setGeometry(QRect(x + x1, y + y1, 25, 25));

        labels[i] = label;
    }
}

LookingForAGoodTime::~LookingForAGoodTime()
{
    delete ui;
}

void LookingForAGoodTime::paintEvent(QPaintEvent *event)
{
    QRect mainRect = ui->centralWidget->geometry();
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();

    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 5));
    painter.drawLine(mainWidth / 2, mainHeight / 2, ui->but_1->geometry().center().x(), ui->but_1->geometry().center().y());

    painter.setPen(QPen(Qt::blue, 10));
    painter.drawLine(mainWidth / 2, mainHeight / 2, (ui->but_2->geometry().center().x() + mainWidth / 2) / 2, (ui->but_2->geometry().center().y() + mainHeight / 2) / 2);
}

void LookingForAGoodTime::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        onMouseEvent("Move", event->pos());
    }

    QMainWindow::mouseMoveEvent(event);
}

void LookingForAGoodTime::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = QPoint(event->pos().x(), event->pos().y() - ui->but_1->geometry().height());

    if (ui->but_1->geometry().contains(pos)) {
        currentLabel = ui->but_1;
        isDragging = true;
    } else if (ui->but_2->geometry().contains(pos)) {
        currentLabel = ui->but_2;
        isDragging = true;
    }

    QMainWindow::mousePressEvent(event);
}

void LookingForAGoodTime::mouseReleaseEvent(QMouseEvent* event)
{
    isDragging = false;
    QMainWindow::mouseReleaseEvent(event);
}

void LookingForAGoodTime::onMouseEvent(const QString &eventName, const QPoint &pos)
{
    QPoint posNew = QPoint(pos.x(), pos.y() - currentLabel->geometry().height());

    for (int i = 0; i < 12; i++) {
        if (labels[i]->geometry().contains(posNew)) {
            currentLabel->setGeometry(labels[i]->geometry().x() + currentLabel->geometry().width() / 2, labels[i]->geometry().y() + currentLabel->geometry().height() / 2, currentLabel->geometry().width(), currentLabel->geometry().height());
            update();
        }
    }
}
