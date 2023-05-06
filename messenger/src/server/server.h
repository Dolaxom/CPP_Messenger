#ifndef MESSENGER_SERVER_H
#define MESSENGER_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class Server : public QTcpServer {
  Q_OBJECT
 public:
  Server();

  QTcpSocket* socket_;  // Новый сокет для каждого нового подключения

 private:
  QVector<QTcpSocket*> sockets_;
  QByteArray byteData_;
  void SendToClient(const QString str);
  quint16 byteBlockSize_;

 public slots:
  // Обработчик новых подключений
  void incomingConnection(qintptr socketDescriptor) override;

  // Обработчик полученных от клиента сообщений
  void slotReadyRead();
};

#endif  // MESSENGER_SERVER_H
