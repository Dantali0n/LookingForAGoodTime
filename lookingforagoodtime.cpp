#include <QMouseEvent>
#include <QPainter>
#include <math.h>

#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"
#include "highscoreservice.h"

// General
int pageIndex = 0;

// Free Play
QLabel* labels[12];
bool isDragging;
QLabel* currentLabel;

LookingForAGoodTime::LookingForAGoodTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LookingForAGoodTime)
{
    ui->setupUi(this);    

    HighscoreService hs = HighscoreService();

    // Create Analog Clock
    QRect mainRect = ui->stackedWidget->geometry();

    const double PI = 3.1415926535;

    // TODO: get correct center + responsive
    int x = mainRect.center().x();
    int y = mainRect.center().y();
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
        label->setParent(ui->freePlay);

        labels[i] = label;
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

void LookingForAGoodTime::paintEvent(QPaintEvent *event)
{
    // Draw the lines only in Free Play
    if (pageIndex == 1) {
        QRect mainRect = ui->freePlay->geometry();
        int mainWidth = mainRect.width();
        int mainHeight = mainRect.height();

        QPainter painter(this);
        painter.setPen(QPen(Qt::red, 5));
        painter.drawLine(mainWidth / 2, mainHeight / 2, ui->but_1->geometry().center().x(), ui->but_1->geometry().center().y());

        painter.setPen(QPen(Qt::blue, 10));
        painter.drawLine(mainWidth / 2, mainHeight / 2, (ui->but_2->geometry().center().x() + mainWidth / 2) / 2, (ui->but_2->geometry().center().y() + mainHeight / 2) / 2);
    }
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
    QPoint posCurrent = QPoint(pos.x(), pos.y() - currentLabel->geometry().height());

    for (int i = 0; i < 12; i++) {
        if (labels[i]->geometry().contains(posCurrent)) {
            currentLabel->setGeometry(labels[i]->geometry().x() + currentLabel->geometry().width() / 2, labels[i]->geometry().y() + currentLabel->geometry().height() / 2, currentLabel->geometry().width(), currentLabel->geometry().height());
            update();
        }
    }
}

void LookingForAGoodTime::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(--pageIndex);
}
