#include <QApplication>

#include "client.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  Client window;
  window.setWindowTitle("Messenger");
  window.resize(500, 600);
  window.show();

  return application.exec();
}
