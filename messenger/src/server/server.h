#ifndef MESSENGER_SERVER_H
#define MESSENGER_SERVER_H

#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QTcpServer {
  Q_OBJECT
 public:
  Server();

 private:
  QMap<qintptr, QTcpSocket*> sockets_;
  QByteArray byteData_;
  void SendToClient(const QString& str);
  quint16 byteBlockSize_;
  uint tempSock;

 public slots:
  // Обработчик новых подключений
  void incomingConnection(qintptr socketDescriptor) override;

  // Обработчик полученных от клиента сообщений
  void slotReadyRead();

  void clientDisconnected();
};

#endif  // MESSENGER_SERVER_H
