#include "server.h"

Server::Server() {
  if (this->listen(QHostAddress::Any, 2323)) {
    qDebug() << "Server run";
  } else {
    qDebug() << "Server error";
  }

  byteBlockSize_ = 0;
}

void Server::incomingConnection(
    qintptr socketDescriptor) {  // Обработчик новых подключений
  socket_ = new QTcpSocket;
  socket_->setSocketDescriptor(socketDescriptor);
  connect(socket_, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
  connect(socket_, &QTcpSocket::disconnected, socket_,
          &QTcpSocket::deleteLater);

  sockets_.push_back(socket_);
  qDebug() << "client connected: " << socketDescriptor;
}

void Server::slotReadyRead() {  // Обработчик полученных от клиента сообщений
  socket_ = (QTcpSocket*)sender();
  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_6_4);
  if (in.status() == QDataStream::Ok) {
    qDebug() << "read ok";
    while (true) {
      if (!byteBlockSize_) {
        if (socket_->bytesAvailable() < 2) {
          break;
        }
        in >> byteBlockSize_;
      }
      if (socket_->bytesAvailable() < byteBlockSize_) {
        break;
      }

      QString str;
      in >> str;
      byteBlockSize_ = 0;
      SendToClient(str);
      break;
    }
  } else {
    qDebug() << "byteData_ stream error";
  }
}

void Server::SendToClient(const QString str) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  // socket_->write(byteData_);
  // Отправка всем пользователям
  for (int i = 0; i < sockets_.size(); i++) {
    sockets_[i]->write(byteData_);
  }
}