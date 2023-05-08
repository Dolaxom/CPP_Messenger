#include "utilities.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>

void Utilities::readPostgresConfig(QString&& filename,
                                   PostgresConfiguration& config) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Can`t find " << filename << " in void Utilities::readPostgresConfig";
    return;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList fields = line.split(' ');
    if (fields.size() != 2) {
      continue;
    }

    if (fields[0] == "HostName")
      config.hostName = fields[1];
    else if (fields[0] == "Port")
      config.hostPort = fields[1].toInt();
    else if (fields[0] == "DatabaseName")
      config.databaseName = fields[1];
    else if (fields[0] == "UserName")
      config.userName = fields[1];
    else if (fields[0] == "Password")
      config.userPassword = fields[1];
  }
}
