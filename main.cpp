#include "lookingforagoodtime.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LookingForAGoodTime w;
    w.show();

    return a.exec();
}
