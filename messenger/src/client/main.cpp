#include <QApplication>

#include "client_view.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  ClientView window;
  window.setWindowTitle("Messenger");
  window.resize(500, 600);
  window.show();

  return application.exec();
}
