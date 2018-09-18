#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"
#include <QMouseEvent>
#include <QPainter>
#include <math.h>

// General
int pageIndex = 0;

// Free Play
QLabel* labels[12];
bool isDragging;
QLabel* currentLabel;
int currentIndex;
int labelpos[2];

LookingForAGoodTime::LookingForAGoodTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LookingForAGoodTime)
{
    ui->setupUi(this);    

    // Create Analog Clock
    for (int i = 0; i < 12; i++) {
        QLabel *label = new QLabel(this);
        label->setObjectName("label_" + QString::number(i + 1));
        label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        label->setText(QString::number(i + 1));
        label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
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
    // Update Free Play ui
    if (pageIndex == 1) {

        // Update the position of the clock numbers
        QRect centralWidgetFrame = ui->centralWidget->geometry();
        int x = centralWidgetFrame.center().x();
        int y = centralWidgetFrame.center().y();

        const double PI = 3.1415926535;
        const int r = 100;
        const int size = 30;
        double angle;
        double x1;
        double y1;

        for (int i = 0; i < 12; i++) {
            angle = (i + 1 + 9) * 360 / 12;
            x1 = r * cos(angle * PI / 180);
            y1 = r * sin(angle * PI / 180);
            labels[i]->setGeometry(x + x1 - size, y + y1 - size, size, size);
        }

        ui->but_1->setGeometry(labels[labelpos[0]]->geometry().center().x() - ui->but_1->geometry().width() / 2, labels[labelpos[0]]->geometry().center().y() - ui->but_1->geometry().height() / 2, ui->but_1->geometry().width(), ui->but_1->geometry().height());
        ui->but_2->setGeometry(labels[labelpos[1]]->geometry().center().x() - ui->but_2->geometry().width() / 2, labels[labelpos[1]]->geometry().center().y() - ui->but_2->geometry().height() / 2, ui->but_2->geometry().width(), ui->but_2->geometry().height());

        // Draw the big and small lines
        QPainter painter(this);

        QRect butOneFrame = ui->but_1->geometry();
        painter.setPen(QPen(Qt::red, 5));
        painter.drawLine(x, y, butOneFrame.center().x() + butOneFrame.width() / 2, butOneFrame.center().y() + butOneFrame.height());

        QRect butTwoFrame = ui->but_2->geometry();
        painter.setPen(QPen(Qt::blue, 10));
        painter.drawLine(x, y, (butTwoFrame.center().x() + butTwoFrame.width() / 2 + x) / 2, (butTwoFrame.center().y() + butTwoFrame.height() + y) / 2);
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
    QPoint pos = event->pos();

    QRect hitboxOne = QRect(ui->but_1->geometry().x() + ui->but_1->geometry().width() / 2, ui->but_1->geometry().y() + ui->but_1->geometry().height(), ui->but_1->geometry().width() * 2, ui->but_1->geometry().height() * 2);
    QRect hitboxTwo = QRect(ui->but_2->geometry().x() + ui->but_2->geometry().width() / 2, ui->but_2->geometry().y() + ui->but_2->geometry().height(), ui->but_2->geometry().width() * 2, ui->but_2->geometry().height() * 2);

    if (hitboxOne.contains(pos)) {
        currentLabel = ui->but_1;
        currentIndex = 0;
        isDragging = true;
    } else if (hitboxTwo.contains(pos)) {
        currentLabel = ui->but_2;
        currentIndex = 1;
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
    for (int i = 0; i < 12; i++) {
        QRect hitbox = QRect(labels[i]->geometry().x() + labels[i]->geometry().width() / 2, labels[i]->geometry().y() + labels[i]->geometry().height() / 1.5, labels[i]->geometry().width(), labels[i]->geometry().height());

        if (hitbox.contains(pos)) {
            labelpos[currentIndex] = i;
            currentLabel->setGeometry(labels[i]->geometry().center().x() - currentLabel->geometry().width() / 2, labels[i]->geometry().center().y() - currentLabel->geometry().height() / 2, currentLabel->geometry().width(), currentLabel->geometry().height());
        }
    }
//    currentLabel->setGeometry(pos.x() - currentLabel->geometry().width(), pos.y() - currentLabel->geometry().height() * 1.5, currentLabel->geometry().width(), currentLabel->geometry().height());
    update();
}

void LookingForAGoodTime::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(--pageIndex);
}
