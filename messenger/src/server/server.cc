#include "server.h"

#include "utilities.h"

Server::Server() : byteBlockSize_{0} {
  if (this->listen(QHostAddress::Any, 2323)) {
    qDebug() << "Server is running on port" << this->serverPort();
  } else {
    qDebug() << "Server failed to start";
  }

  initPostgres();
}

// Обработчик новых подключений
void Server::incomingConnection(qintptr socketDescriptor) {
  auto* socket = new QTcpSocket;
  socket->setSocketDescriptor(socketDescriptor);
  connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
  connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
  connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

  sockets_.insert(socket->socketDescriptor(), socket);
  qDebug() << "client connected: " << socket->socketDescriptor();
}

// Обработчик полученных от клиента сообщений
void Server::slotReadyRead() {
  auto* socket = qobject_cast<QTcpSocket*>(sender());
  if (!socket) {
    qDebug("Wrong socket");
    return;
  }
  QDataStream in(socket);
  in.setVersion(QDataStream::Qt_6_4);
  if (in.status() == QDataStream::Ok) {
    while (true) {
      if (!byteBlockSize_) {
        if (socket->bytesAvailable() < 2) {
          break;
        }
        in >> byteBlockSize_;
      }

      if (socket->bytesAvailable() < byteBlockSize_) {
        break;
      }

      QString str;
      int type = 0;
      in >> type;

      switch (type) {
        case 0: {
          QString senderName, receiverName, message;
          in >> senderName >> receiverName >> message;

          QString queryStr =
              QString(
                  "INSERT INTO messages (sender_name, receiver_name, message) "
                  "VALUES ('%1', '%2', '%3')")
                  .arg(senderName)
                  .arg(receiverName)
                  .arg(message);
          QSqlQuery query(queryStr);
          break;
        }
        case 1: {
          QString login, password;
          in >> login >> password;
          qDebug() << socket->socketDescriptor() << " trying auth: " << login;

          QString queryStr = QString(
                                 "SELECT COUNT(*) FROM users WHERE "
                                 "username='%1' AND password='%2'")
                                 .arg(login)
                                 .arg(password);
          QSqlQuery query(queryStr);
          if (query.next()) {
            int count = query.value(0).toInt();
            str = QString::number(count);
            SendToClient(str, 1, socket);
          }
          break;
        }
        case 2: {
          QString login, password;
          in >> login >> password;

          QString queryStrCheckUser =
              QString("SELECT COUNT(*) FROM users WHERE username='%1'")
                  .arg(login);
          QSqlQuery queryCheckUser(queryStrCheckUser);
          int count = 0;
          if (queryCheckUser.next()) {
            count = queryCheckUser.value(0).toInt();
          }
          if (!count) {
            QString queryStrInsert =
                QString(
                    "INSERT INTO users (username, password) VALUES('%1', '%2')")
                    .arg(login)
                    .arg(password);
            QSqlQuery queryInsert(queryStrInsert);
            str = "OK";
          } else {
            str = "BAD";
          }

          SendToClient(str, 2, socket);
          break;
        }
        case 3: {
          QString senderName, receiverName;
          in >> senderName >> receiverName;

          QString queryStr =
              QString(
                  "SELECT sender_name, message FROM messages WHERE "
                  "sender_name='%1' AND receiver_name='%2' OR sender_name='%3' "
                  "AND receiver_name='%4' ORDER BY sent_time ASC")
                  .arg(senderName)
                  .arg(receiverName)
                  .arg(receiverName)
                  .arg(senderName);
          QSqlQuery query(queryStr);
          while (query.next()) {
            str += query.value(0).toString() + ": " +
                   query.value(1).toString() + '\n';
          }
          SendToClient(str, 3, socket);
          break;
        }
        default:
          break;
      }

      byteBlockSize_ = 0;
      break;
    }
  } else {
    qDebug() << "Message stream error";
  }
}

void Server::SendToClient(const QString& str, const int type,
                          QTcpSocket* senderSocket) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);

  out << quint16(0) << type << tempSock_ << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));

  if (!type) {
    auto sock = sockets_.find(tempSock_);
    sock.value()->write(byteData_);
    qDebug() << sock.value()->socketDescriptor();
  } else if (type == 1) {
    senderSocket->write(byteData_);
  } else if (type == 2) {
    senderSocket->write(byteData_);
  } else if (type == 3) {
    senderSocket->write(byteData_);
  }
}

void Server::clientDisconnected() {
  auto* socket = qobject_cast<QTcpSocket*>(sender());
  if (socket) {
    if (socket->error() != QAbstractSocket::RemoteHostClosedError) {
      qDebug() << "client disconnected with error: " << socket->errorString();
    } else {
      qDebug() << "client disconnected: " << socket->socketDescriptor();
    }
    sockets_.remove(socket->socketDescriptor());
  }
}

void Server::initPostgres() {
  db_ = QSqlDatabase::addDatabase("QPSQL");

  PostgresConfiguration psqlConf;
  Utilities::readPostgresConfig("postgres.conf", psqlConf);

  db_.setHostName(psqlConf.hostName);
  db_.setPort(psqlConf.hostPort);
  db_.setDatabaseName(psqlConf.databaseName);
  db_.setUserName(psqlConf.userName);
  db_.setPassword(psqlConf.userPassword);

  if (!db_.open()) {
    qDebug() << "Error opening database:" << db_.lastError().text();
    return;
  }

  qDebug() << "Connected to PostgreSQL database.";
}

Server::~Server() { db_.close(); }
