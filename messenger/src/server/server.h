#ifndef MESSENGER_SERVER_H
#define MESSENGER_SERVER_H

#include <QMap>
#include <QSqlError>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class Server : public QTcpServer {
  Q_OBJECT
 public:
  Server();
  ~Server();

 private:
  void initPostgres();
  QMap<qintptr, QTcpSocket *> sockets_;
  QByteArray byteData_;
  void SendToClient(const QString &str, int type,
                    QTcpSocket *senderSocket = nullptr);
  quint16 byteBlockSize_;
  QSqlDatabase db;
  uint tempSock;

 public slots:
  // Обработчик новых подключений
  void incomingConnection(qintptr socketDescriptor) override;

  // Обработчик полученных от клиента сообщений
  void slotReadyRead();

  void clientDisconnected();
};

#endif  // MESSENGER_SERVER_H
