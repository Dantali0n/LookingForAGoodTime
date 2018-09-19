#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"
#include <QMouseEvent>
#include <QPainter>
#include <math.h>

// The current page
int pageIndex = 0;

// The clock arm which is/was being dragged.
QLabel* currentClockArm;

// Value of pi
const double CONST_PI = 3.1415926535;

// Radius of the clock
const int CONST_RADIUS = 100;

// The width and height of the arm endpoint
const int CONST_SIZE = 30;

// Holds the Labels with the numbers of the clock from 1 to 12
QLabel* clockNumberLabels[12];

// Determines if the user is dragging any clockarm
bool isDragging;

// The angles of clock hands
double clockHandBigRectAngle = 0;
double clockHandSmallRectAngle = 0;


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
        QPoint center = getCentralWidgetFrameCenterPoint();

        // Draw the clock number.
        setClockNumbersGeometry(center);

        // Setup painter
        QPainter painter(this);

        // Draw clock circle
        painter.drawEllipse(QPointF(center.x(), center.y()), 100, 100);

        // Draw big clock hand
        painter.setPen(QPen(Qt::red, 5));
        QPoint finalPoint = setFinalPoint(ui->clockHandBig, 60, clockHandBigRectAngle);
        painter.drawLine(center.x(), center.y(), finalPoint.x(), finalPoint.y());

        // Draw small clock hand
        painter.setPen(QPen(Qt::blue, 10));
        finalPoint = setFinalPoint(ui->clockHandSmall, 60 * 60, clockHandSmallRectAngle);
        painter.drawLine(center.x(), center.y(), (finalPoint.x() + center.x()) / 2, (finalPoint.y() + center.y()) / 2);
    }
}

// Mouse Moving
void LookingForAGoodTime::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        QPoint center = getCentralWidgetFrameCenterPoint();
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();

        // Calculates the angle of the current arm
        double angle = atan2(center.x() - mouseX, mouseY - center.y()) + CONST_PI;

        // Set to the appropriate clock arm
        if (currentClockArm == ui->clockHandBig) {
            clockHandBigRectAngle = angle;
        } else {
            clockHandSmallRectAngle = angle;
        }

        // Force the paint event
        update();
    }

    QMainWindow::mouseMoveEvent(event);
}

// Mouse Press
void LookingForAGoodTime::mousePressEvent(QMouseEvent* event)
{
    // Get mouse press region
    QRect hitboxBig = getHitbox(ui->clockHandBig);
    QRect hitboxSmall = getHitbox(ui->clockHandSmall);

    // Check is the mouse press is in the region
    bool didPressBig = hitboxBig.contains(event->pos());
    bool didPressSmall = hitboxSmall.contains(event->pos());

    // Set the current clock arm accordingly
    if (didPressBig) {
        currentClockArm = ui->clockHandBig;
        isDragging = true;

    } else if (didPressSmall) {
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

QPoint LookingForAGoodTime::setFinalPoint(QLabel *arm, int sections, double angle)
{
    QPoint center = getCentralWidgetFrameCenterPoint();

    // For extra reference:
    // http://www.cplusplus.com/reference/cstdlib/div/

    // Degrees per section
    double degrees = 2 * CONST_PI / sections;

    // The quotient. Determines the actual section. (0 - 59 or 0 - 3599)
    int quot = angle / degrees;

    // The remainder. Needed to check where in the section the arm is.
    double rem = angle - quot * degrees;

    // Change the quot to the next one, when the arm is halfway through.
    if (rem > CONST_PI / sections) {
        quot++;
    }

    // Change the last number to 0.
    if (quot == sections) {
        quot = 0;
    }

    double quotDegrees = quot * degrees;
    double quotDegreesSin = CONST_RADIUS * sin(quotDegrees);
    double quotDegreesCos = CONST_RADIUS * cos(quotDegrees);

    // The actual endpoint coordinates.
    int x1 = center.x() + quotDegreesSin;
    int y1 = center.y() - quotDegreesCos;

    // Set the visual arm enpoint positions.
    int x1Arm = x1 - CONST_SIZE / 1.5;
    int y1Arm = y1 - CONST_SIZE * 1.25;
    arm->setGeometry(x1Arm, y1Arm, arm->geometry().width(), arm->geometry().height());

    // Also return the actual point.
    return QPoint(x1, y1);
}

QPoint LookingForAGoodTime::getCentralWidgetFrameCenterPoint()
{
    QRect centralWidgetRect = ui->centralWidget->geometry();
    int x = centralWidgetRect.center().x();
    int y = centralWidgetRect.center().y();
    return QPoint(x, y);
}

QRect LookingForAGoodTime::getHitbox(QLabel *arm)
{
    QRect rect = arm->geometry();

    // Convert the hitbox to the same rect as the visual arm endpoint
    int x = rect.x() + rect.width() / 2;
    int y = rect.y() + rect.height();
    int width = rect.width() * 2;
    int height = rect.height() * 2;

    return QRect(x, y, width, height);
}

void LookingForAGoodTime::setClockNumbersGeometry(QPoint center)
{
    // Circle has 360°. A clock has 12 hours. So each section is 30°.
    int secDegrees = 360 / 12;

    // length from the center + half size of the arm endpoint
    int radius = CONST_RADIUS + CONST_SIZE / 2;

    double degrees = CONST_PI / 180;

    double angle;
    int x;
    int y;
    double x1;
    double y1;

    // Calculates the positions of the labels
    for (int i = 0; i < 12; i++) {

        // Angle 0° starts at 3 hour. So add 9 to make it start at 12 hour. However, The first labels is 1 hour, so we need to add 1.
        int section = i + 9 + 1;

        angle = section * secDegrees;

        // Calculates the endpoints
        x1 = radius * cos(angle * degrees);
        y1 = radius * sin(angle * degrees);
        x = center.x() + x1 - CONST_SIZE / 2;
        y = center.y() + y1 - CONST_SIZE * 1.25;

        clockNumberLabels[i]->setGeometry(x, y, CONST_SIZE, CONST_SIZE);
    }
}
