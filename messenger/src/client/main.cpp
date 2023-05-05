#include "messengerbase.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MessengerBase window;
    window.setWindowTitle("Messenger");
    window.resize(500, 600);
    window.show();
    return application.exec();
}
