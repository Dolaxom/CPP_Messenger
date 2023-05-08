#ifndef MESSENGER_UTILITIES_H
#define MESSENGER_UTILITIES_H

#include <QString>

struct PostgresConfiguration {
  QString hostName;
  QString databaseName;
  QString userName;
  QString userPassword;
  int hostPort;
};

class Utilities {
 public:
  static void readPostgresConfig(QString&& filename,
                                 PostgresConfiguration& config);
};

#endif  // MESSENGER_UTILITIES_H
