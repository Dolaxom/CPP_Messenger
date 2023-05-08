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

      QString str;

      in >> tempSock >> str;
      byteBlockSize_ = 0;
      SendToClient(str);
      qDebug() << "Read " << str << " from " << socket->socketDescriptor();
      break;
    }
  } else {
    qDebug() << "Message stream error";
  }
}

void Server::SendToClient(const QString& str) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);

  out << quint16(0) << tempSock << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));

  auto sock = sockets_.find(tempSock);
  sock.value()->write(byteData_);
  qDebug() << sock.value()->socketDescriptor();
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

  QSqlQuery query("SELECT * FROM users");
  while (query.next()) {
    int id = query.value(0).toInt();
    QString username = query.value(1).toString();
    QString password = query.value(2).toString();
    qDebug() << id << username << password;
  }
}

Server::~Server() { db.close(); }
