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
