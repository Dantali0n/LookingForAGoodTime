#include <QMouseEvent>
#include <QPainter>
#include <QStringList>
#include <QStringListModel>

#include <sstream>
#include <math.h>

#include "highscoreservice.h"
#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"

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
int hours = 0;
int minutes = 0;
int prevQuotHours = 0;
int prevQuotMinutes = 0;

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

    HighscoreService hs = HighscoreService();

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



/**
 * @brief closes the application when the exit button is pressed.
 */
void LookingForAGoodTime::on_exitButton_clicked()
{
    QCoreApplication::quit();
}

void LookingForAGoodTime::on_freePlayButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 1);
}

void LookingForAGoodTime::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(--pageIndex);
}

void LookingForAGoodTime::on_highscoresButton_clicked()
{
    HighscoreService hs = HighscoreService();
    std::vector<HighscoreModel*> top10 = hs.getTop10();
    QStringListModel* model = new QStringListModel(this);
    QStringList list;

    uint64_t rank = 1;
    for (std::vector<HighscoreModel*>::iterator it = top10.begin() ; it != top10.end(); ++it)
    {
        std::ostringstream convert;
        convert << rank << " - ";
        convert << (*it)->name.toStdString() << " - ";
        convert << (*it)->score;
        list << QString::fromStdString(convert.str());
        delete (*it);
        rank++;
    }
    model->setStringList(list);
    ui->highscoreList->setModel(model);
    ui->stackedWidget->setCurrentIndex(2);
}

void LookingForAGoodTime::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



// Draw ui
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

        // Update time after dragging an arm
        if (isDragging) {
            if (currentClockArm == ui->clockHandBig) {
                int quot = clockHandBigRectAngle / (2 * CONST_PI / 60);
                int prev = prevQuotHours;

                if (prev <= 60 && quot >= 0 && prev >= 40 && quot <= 20) {
                    if (hours == 23) {
                        hours = -1;
                    }
                    minutes = 0;
                    hours++;
                } else if (prev >= 0 && quot <= 60 && prev <= 20 && quot >= 40) {
                    if (hours == 0) {
                        hours = 24;
                    }
                    minutes = 59;
                    hours--;
                } else if (quot > prev) {
                    minutes = quot;
                } else if (quot < prev) {
                    minutes = quot;
                }

                prevQuotHours = quot;

                // Update digital clock
                ui->timeEdit->setTime(QTime(hours, minutes, 0,0));

            } else if (currentClockArm == ui->clockHandSmall) {
                int quot = clockHandSmallRectAngle / (2 * CONST_PI / (60 * 60));
                int x = (hours + 1) * 300;
                int mPrev = prevQuotMinutes % x / 5;
                int min = quot % x / 5;
                int qwer = mPrev % 60;
                int asdf = min % 60;

                if (qwer <= 59 && asdf >= 0 && qwer >= 39 && asdf <= 20) {
                    hours++;
                    if (hours == 24) {
                        hours = 0;
                    }
                    minutes = 0;
                } else if (qwer >= 0 && asdf <= 59 && qwer <= 20 && asdf >= 39) {
                    hours--;
                    if (hours == -1) {
                        hours = 23;
                    }
                    minutes = 59;
                } else if (quot != mPrev) {
                    minutes = min % 60;
                }

                prevQuotMinutes = quot;

                // Update digital clock
                ui->timeEdit->setTime(QTime(hours, minutes, 0,0));
            }
        }

        // Draw the big arm
        QLabel* arm = ui->clockHandBig;
        int hh = hours;
        int mm = minutes;
        int sections = 60;
        double rads = 2 * CONST_PI;
        double bigArmSec = rads / sections;
        double bigArmAngle = bigArmSec * mm;
        int quot = bigArmAngle / bigArmSec;
        double rem = bigArmAngle - quot * bigArmSec;
        if (rem > CONST_PI / sections) {
            quot++;
        }
        if (quot == sections) {
            quot = 0;
        }
        double quotDegrees = quot * bigArmSec;
        double quotDegreesSin = CONST_RADIUS * sin(quotDegrees);
        double quotDegreesCos = CONST_RADIUS * cos(quotDegrees);
        int x1 = center.x() + quotDegreesSin;
        int y1 = center.y() - quotDegreesCos;
        int x1Arm = x1 - CONST_SIZE / 1.5;
        int y1Arm = y1 - CONST_SIZE * 1.25;
        arm->setGeometry(x1Arm, y1Arm, arm->geometry().width(), arm->geometry().height());

        painter.setPen(QPen(Qt::red, 5));
        painter.drawLine(center.x(), center.y(), x1, y1);


        // Draw the small arm
        arm = ui->clockHandSmall;
        int xxx = hh * 300 + mm * 5;
        sections = 60 * 60;

        bigArmSec = rads / sections;
        bigArmAngle = bigArmSec * xxx;
        quot = bigArmAngle / bigArmSec;
        rem = bigArmAngle - quot * bigArmSec;
        if (rem > CONST_PI / sections) {
            quot++;
        }
        if (quot == sections) {
            quot = 0;
        }
        quotDegrees = quot * bigArmSec;
        quotDegreesSin = CONST_RADIUS * sin(quotDegrees);
        quotDegreesCos = CONST_RADIUS * cos(quotDegrees);
        x1 = center.x() + quotDegreesSin;
        y1 = center.y() - quotDegreesCos;
        x1Arm = x1 - CONST_SIZE / 1.5;
        y1Arm = y1 - CONST_SIZE * 1.25;
        arm->setGeometry(x1Arm, y1Arm, arm->geometry().width(), arm->geometry().height());

        painter.setPen(QPen(Qt::blue, 10));
        painter.drawLine(center.x(), center.y(), (x1 + center.x()) / 2, (y1 + center.y()) / 2);

        // Update the label with the correct pronunciation of the time
        setTimeLabel();
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

        update();
    }
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
}

// Mouse Release
void LookingForAGoodTime::mouseReleaseEvent(QMouseEvent* event)
{
    isDragging = false;
}

// Key Press
void LookingForAGoodTime::keyPressEvent(QKeyEvent *event)
{
    if (pageIndex == 1) {
        // Update the time after pressing enter/return in th time box
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            isDragging = false;
            hours = ui->timeEdit->time().hour();
            minutes = ui->timeEdit->time().minute();
            update();
        }
    }
}


// Returns the center of the central widget.
QPoint LookingForAGoodTime::getCentralWidgetFrameCenterPoint()
{
    QRect centralWidgetRect = ui->centralWidget->geometry();
    int x = centralWidgetRect.center().x();
    int y = centralWidgetRect.center().y();
    return QPoint(x, y);
}

// Returns the mouse press area of the arm endpoint.
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

// Updates the location of the clock numbers.
void LookingForAGoodTime::setClockNumbersGeometry(QPoint center)
{
    // Circle has 360Â°. A clock has 12 hours. So each section is 30Â°.
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

        // Angle 0Â° starts at 3 hour. So add 9 to make it start at 12 hour. However, The first labels is 1 hour, so we need to add 1.
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

// Returns the sting equivalent of the hour.
QString LookingForAGoodTime::getHourString(int time)
{
    switch (time) {
        case 1:     case 13:            return "een";
        case 2:     case 14:            return "twee";
        case 3:     case 15:            return "drie";
        case 4:     case 16:            return "vier";
        case 5:     case 17:            return "vijf";
        case 6:     case 18:            return "zes";
        case 7:     case 19:            return "zeven";
        case 8:     case 20:            return "acht";
        case 9:     case 21:            return "negen";
        case 10:    case 22:            return "tien";
        case 11:    case 23:            return "elf";
        default:                        return "twaalf";
    }
}

// Returns the sting equivalent of the minute.
// NOTE: When the time is 15 or 30, retuns 'kwart' and 'half'.
QString LookingForAGoodTime::getMinuteString(int time)
{
    switch (time) {
        case 1:     case 29:            return "een";
        case 2:     case 28:            return "twee";
        case 3:     case 27:            return "drie";
        case 4:     case 26:            return "vier";
        case 5:     case 25:            return "vijf";
        case 6:     case 24:            return "zes";
        case 7:     case 23:            return "zeven";
        case 8:     case 22:            return "acht";
        case 9:     case 21:            return "negen";
        case 10:    case 20:            return "tien";
        case 11:    case 19:            return "elf";
        case 12:    case 18: case 0:    return "twaalf";
        case 13:    case 17:            return "dertien";
        case 14:    case 16:            return "viertien";
        default:                        return "kwart";
    }
}

// Updates the time label with the new time.
void LookingForAGoodTime::setTimeLabel()
{
    int h = hours;
    int m = minutes;

    // Change the hour to the next hour when the minutes passed 15 min.
    if (m > 15) {
        h++;
    }

    // Converts the ints to a readable string
    QString hoursString = getHourString(h);
    QString minutesString = getMinuteString(m % 30);


    // Exact '[h] uur'
    if (m == 0) {
        ui->clockNameLabel->setText(hoursString + " uur");
        return;
    }

    // Exact 'half [h]'
    if (m == 30) {
        ui->clockNameLabel->setText("half " + hoursString);
        return;
    }

    QString indicatie = "";
    if (m < 16) {
        indicatie = "over";
    } else if (m < 30) {
        indicatie = "voor half";
    } else if (m < 45) {
        indicatie = "over half";
    } else if (m < 60) {
        indicatie = "voor";
    }

    ui->clockNameLabel->setText(minutesString + " " + indicatie + " " + hoursString);
}

void LookingForAGoodTime::on_challengesBackbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 0);
}

void LookingForAGoodTime::on_challengesRandomButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 4);
}

void LookingForAGoodTime::on_challengesAnalogButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 5);
}

void LookingForAGoodTime::on_challengesDigitalButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 6);
}

void LookingForAGoodTime::on_challengesSpeakButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 7);
}

void LookingForAGoodTime::on_challengeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 3);
}

void LookingForAGoodTime::on_randomBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 3);
}

void LookingForAGoodTime::on_analogBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 3);
}

void LookingForAGoodTime::on_digitalBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 3);
}

void LookingForAGoodTime::on_speachBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(pageIndex = 3);
}
