#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"
#include <QMouseEvent>
#include <QPainter>
#include <math.h>

// General
int pageIndex = 0;

// The clock arm which is/was being dragged.
QLabel* currentClockArm;

// Constants
const double CONST_PI = 3.1415926535;
const int CONST_RADIUS = 100;
const int CONST_SIZE = 30;
int hours = 0;
int minutes = 0;

// Holds the Labels with the numbers of the clock from 1 to 12
QLabel* clockNumberLabels[12];

// Determines if the user is dragging any clockarm
bool isDragging;

// The coordinates of the end of the clock hands
QPoint clockHandBigRectPoint;
QPoint clockHandSmallRectPoint;


LookingForAGoodTime::LookingForAGoodTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LookingForAGoodTime)
{
    ui->setupUi(this);    

    // Creates the analog clock numbers
    for (int i = 0; i < 12; i++) {
        QLabel *label = new QLabel(this);
        label->setText(QString::number(i + 1));
        label->setParent(ui->freePlay);
        clockNumberLabels[i] = label;
    }

    // Set current page to menu
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

LookingForAGoodTime::~LookingForAGoodTime()
{
    delete ui;
}

void LookingForAGoodTime::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    }

    if (event->key() == Qt::Key_D)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    }
}

/**
 * @brief closes the application when the exit button is pressed.
 */
void LookingForAGoodTime::on_exitButton_clicked()
{
    QCoreApplication::quit();
}

void LookingForAGoodTime::on_freePlayButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(++pageIndex);
}

// Draw
void LookingForAGoodTime::paintEvent(QPaintEvent *event)
{
    // Update Free Play ui
    if (pageIndex == 1) {
        const double PI = CONST_PI;
        const int r = CONST_RADIUS;
        const int size = CONST_SIZE;

        QRect centralWidgetFrame = ui->centralWidget->geometry();
        int x = centralWidgetFrame.center().x();
        int y = centralWidgetFrame.center().y();

        double angle;
        double x1;
        double y1;

        // Draw clock numbers
        for (int i = 0; i < 12; i++) {
            angle = (i + 1 + 9) * 360 / 12;
            x1 = (r + size/2) * cos(angle * PI / 180);
            y1 = (r + size/2) * sin(angle * PI / 180);
            clockNumberLabels[i]->setGeometry(x + x1 - size / 2, y + y1 - size * 1.25, size, size);
        }

        // Setup painter
        QPainter painter(this);
        painter.drawEllipse(QPointF(x,y), 100, 100);

        // Draw big clock hand
        painter.setPen(QPen(Qt::red, 5));
        painter.drawLine(x, y, clockHandBigRectPoint.x(), clockHandBigRectPoint.y());

        // Draw small clock hand
        painter.setPen(QPen(Qt::blue, 10));
        painter.drawLine(x, y, (clockHandSmallRectPoint.x() + x) / 2, (clockHandSmallRectPoint.y() + y) / 2);
        //ui->timeEdit->setTime(QTime::currentTime());
        //QTime digitalTime = ui->timeEdit->time();
    }
}

// Mouse Moving
void LookingForAGoodTime::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        // Calculates the position of the current arm
        const double PI = CONST_PI;
        const int r = CONST_RADIUS;
        const int size = CONST_SIZE;

        int sec = currentClockArm == ui->clockHandBig ? 60 : 60 * 60;

        int mouseX = event->pos().x();
        int mouseY = event->pos().y();

        QRect centralWidgetFrame = ui->centralWidget->geometry();
        int x = centralWidgetFrame.center().x();
        int y = centralWidgetFrame.center().y();

        double angle = atan2(x - mouseX, mouseY - y) + PI;
        double denom = 2 * PI / sec;
        int quot = angle / denom;
        double rem = angle - quot * denom;

        if (rem > PI / sec) {
            quot++;
        }

        if (quot == sec) {
            quot = 0;
        }

        int x1 = x - size / 1.5 + r * sin(quot * 2 * PI / sec);
        int y1 = y - size * 1.25 - r * cos(quot * 2 * PI / sec);

        currentClockArm->setGeometry(x1, y1, currentClockArm->geometry().width(), currentClockArm->geometry().height());

        QPoint finalPoint = QPoint(x + r * sin(quot * 2 * PI / sec), y - r * cos(quot * 2 * PI / sec));
        if (currentClockArm == ui->clockHandBig) {
            clockHandBigRectPoint = finalPoint;
            ui->timeEdit->setTime(QTime(quot, 0, 0,0));
        } else {
            clockHandSmallRectPoint = finalPoint;
            hours = quot/300;
            if(hours > 0){
                quot -= 300 * hours;
            }
            ui->timeEdit->setTime(QTime(hours, quot/5, 0,0));
        }


        update();

    }

    QMainWindow::mouseMoveEvent(event);
}

// Mouse Press
void LookingForAGoodTime::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    QRect hitboxBig = QRect(ui->clockHandBig->geometry().x() + ui->clockHandBig->geometry().width() / 2, ui->clockHandBig->geometry().y() + ui->clockHandBig->geometry().height(), ui->clockHandBig->geometry().width() * 2, ui->clockHandBig->geometry().height() * 2);
    QRect hitboxSmall = QRect(ui->clockHandSmall->geometry().x() + ui->clockHandSmall->geometry().width() / 2, ui->clockHandSmall->geometry().y() + ui->clockHandSmall->geometry().height(), ui->clockHandSmall->geometry().width() * 2, ui->clockHandSmall->geometry().height() * 2);

    if (hitboxBig.contains(pos)) {
        currentClockArm = ui->clockHandBig;
        isDragging = true;
    } else if (hitboxSmall.contains(pos)) {
        currentClockArm = ui->clockHandSmall;
        isDragging = true;
    }

    QMainWindow::mousePressEvent(event);
}

// Mouse Release
void LookingForAGoodTime::mouseReleaseEvent(QMouseEvent* event)
{
    isDragging = false;
    QMainWindow::mouseReleaseEvent(event);
}

void LookingForAGoodTime::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(--pageIndex);
}

void LookingForAGoodTime::on_challengeButton_clicked()
{

}

void LookingForAGoodTime::on_timeEdit_timeChanged(const QTime &time)
{

}
