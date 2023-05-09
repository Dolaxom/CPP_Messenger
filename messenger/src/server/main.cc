#include <QCoreApplication>

#include "server.h"

int main(int argc, char *argv[]) {
  QCoreApplication application(argc, argv);
  Server server;

  return application.exec();
}
