#ifndef MESSENGER_MESSENGER_CONTROLLER_H
#define MESSENGER_MESSENGER_CONTROLLER_H

#include <QTcpSocket>

class MessengerController {
  Q_OBJECT
 public:
  MessengerController();

  QTcpSocket* getSocket() const noexcept { return socket_; }
  QByteArray getByteData() const noexcept { return byteData_; }

 private:
  QTcpSocket* socket_;
  QByteArray byteData_;
};

#endif  // MESSENGER_MESSENGER_CONTROLLER_H
