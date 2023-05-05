#include "messengerbase.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MessengerBase w;
    w.show();
    return a.exec();
}
