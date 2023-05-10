#ifndef MESSENGER_CLIENT_DATA_H
#define MESSENGER_CLIENT_DATA_H

#include <QString>

struct ClientData {
  ClientData() = default;
  ClientData(const ClientData&) = delete;
  void operator=(const ClientData&) = delete;

  QString username;
};

#endif  // MESSENGER_CLIENT_DATA_H
