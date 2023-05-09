#include "server.h"

#include "utilities.h"

Server::Server() {
  if (this->listen(QHostAddress::Any, 2323)) {
    qDebug() << "Server run";
  } else {
    qDebug() << "Server error";
  }

  byteBlockSize_ = 0;
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

      int type = 0;
      QString str;
      in >> type;
      if (type == 1) {
        QString login, password;
        in >> login >> password;
        qDebug() << socket->socketDescriptor() << " trying auth: " << login;

        QSqlQuery query("SELECT COUNT(*) FROM users where username='" + login +
                        "' AND password='" + password + "'");
        if (query.next()) {
          int count = query.value(0).toInt();
          str = QString::number(count);
          SendToClient(str, 1, socket);
        }
      } else if (type == 2) {
        QString login, password;
        in >> login >> password;
        qDebug() << socket->socketDescriptor() << " trying register: " << login;

        QSqlQuery queryCheckUser("SELECT COUNT(*) FROM users where username='" +
                                 login + "'");
        int count = 0;
        if (queryCheckUser.next()) {
          count = queryCheckUser.value(0).toInt();
        }
        if (!count) {
          QSqlQuery query("INSERT INTO users (username, password) VALUES('" +
                          login + "', '" + password + "');");
          str = "OK";
        } else {
          str = "BAD";
        }

        SendToClient(str, 2, socket);
      } else if (type == 3) {
        QString senderName, receiverName;
        in >> senderName >> receiverName;

        qDebug() << socket->socketDescriptor()
                 << " trying load message history.";
        QSqlQuery query(
            "SELECT sender_name, message FROM messages WHERE sender_name = '" +
            senderName + "' AND receiver_name = '" + receiverName +
            "' OR sender_name = '" + receiverName + "' AND receiver_name = '" +
            senderName + "' ORDER BY sent_time ASC;");

        while (query.next()) {
          str += query.value(0).toString() + ": " + query.value(1).toString() +
                 '\n';
        }
        SendToClient(str, 3, socket);
      } else if (!type) {
        QString senderName, receiverName, message;
        in >> senderName >> receiverName >> message;

        QSqlQuery query("INSERT INTO messages (sender_name, receiver_name, message) VALUES ('" + senderName + "', '" + receiverName + "', '" + message + "');");
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

  out << quint16(0) << type << tempSock << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));

  if (!type) {
    auto sock = sockets_.find(tempSock);
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
    qDebug() << "client disconnected: " << socket->socketDescriptor();
    sockets_.remove(socket->socketDescriptor());
  }
}

void Server::initPostgres() {
  db = QSqlDatabase::addDatabase("QPSQL");

  PostgresConfiguration psqlConf;
  Utilities::readPostgresConfig("postgres.conf", psqlConf);

  db.setHostName(psqlConf.hostName);
  db.setPort(psqlConf.hostPort);
  db.setDatabaseName(psqlConf.databaseName);
  db.setUserName(psqlConf.userName);
  db.setPassword(psqlConf.userPassword);

  if (!db.open()) {
    qDebug() << "Error opening database:" << db.lastError().text();
  }
}

Server::~Server() { db.close(); }
