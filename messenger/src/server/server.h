#ifndef MESSENGER_SERVER_H
#define MESSENGER_SERVER_H

#include <QMap>
#include <QSqlError>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class Server : public QTcpServer {
  Q_OBJECT
 public:
  Server();
  ~Server();

 private:
  void initPostgres();
  QSet<QTcpSocket *> sockets_;
  QByteArray byteData_;
  void SendToClient(
      const QString &str, int type,
      QVector<QTcpSocket *> senderSockets = QVector<QTcpSocket *>());
  quint16 byteBlockSize_;
  QSqlDatabase db_;
  uint tempSock_;

  void readMessage(QTcpSocket *socket, QDataStream &in, QString &str);
  void readAuthentication(QTcpSocket *socket, QDataStream &in, QString &str);
  void readRegistration(QTcpSocket *socket, QDataStream &in, QString &str);
  void readMessageHistory(QTcpSocket *socket, QDataStream &in, QString &str);

 public slots:
  // Обработчик новых подключений
  void incomingConnection(qintptr socketDescriptor) override;

  // Обработчик полученных от клиента сообщений
  void slotReadyRead();

  void clientDisconnected();
};

#endif  // MESSENGER_SERVER_H
