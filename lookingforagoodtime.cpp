#include "lookingforagoodtime.h"
#include "ui_lookingforagoodtime.h"

LookingForAGoodTime::LookingForAGoodTime(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LookingForAGoodTime)
{
    ui->setupUi(this);
}

LookingForAGoodTime::~LookingForAGoodTime()
{
    delete ui;
}

void LookingForAGoodTime::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    }

    if(event->key() == Qt::Key_D)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    }
}

/**
 * @brief closes the application when the exit button is pressed.
 */
void LookingForAGoodTime::on_exitButton_clicked()
{
    quick_exit(0);
}
