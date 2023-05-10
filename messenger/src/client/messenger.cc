#include "messenger.h"

#include "../types.h"

Messenger::Messenger(QWidget* parent)
    : QWidget(parent), byteBlockSize_{0} {
  clientData_ = new ClientData();

  nicknameLabel_ = new QLabel();
  nicknameLabel_->setAlignment(Qt::AlignCenter);

  chatBox_ = new QTextEdit();
  chatBox_->setReadOnly(true);

  messageBox_ = new QTextEdit();
  messageBox_->setFixedHeight(30);

  findUserLayout_ = new QHBoxLayout();
  findUserButton_ = new QPushButton("Find");

  sendButton_ = new QPushButton("Send");
  connect(sendButton_, &QPushButton::clicked, this,
          &Messenger::onSendButtonClicked);
  connect(findUserButton_, &QPushButton::clicked, this,
          &Messenger::onFindUserButtonClicked);

  QLabel* targetUserLabel = new QLabel("Enter target username:");
  targetUser_ = new QTextEdit();
  targetUser_->setFixedHeight(30);

  chatLayout_ = new QVBoxLayout();
  findUserLayout_->addWidget(targetUser_);
  findUserLayout_->addWidget(findUserButton_);
  chatLayout_->addWidget(targetUserLabel);
  chatLayout_->addLayout(findUserLayout_);
  chatLayout_->addWidget(targetUser_);
  chatLayout_->addWidget(chatBox_);

  messageLayout_ = new QHBoxLayout();
  messageLayout_->addWidget(messageBox_);
  messageLayout_->addWidget(sendButton_);

  mainLayout_ = new QVBoxLayout();
  mainLayout_->addWidget(nicknameLabel_);
  mainLayout_->addLayout(chatLayout_);
  mainLayout_->addLayout(messageLayout_);

  setLayout(mainLayout_);

  serverSocket_ = new QTcpSocket(this);
  serverSocket_->setSocketOption(QAbstractSocket::LowDelayOption, 1);

  connect(serverSocket_, &QTcpSocket::readyRead, this,
          &Messenger::slotReadyRead);
  connect(serverSocket_, &QTcpSocket::disconnected, serverSocket_,
          &QTcpSocket::deleteLater);

  serverSocket_->connectToHost("127.0.0.1", 2323);
}

Messenger::~Messenger() { delete clientData_; }

// Приём данных
void Messenger::slotReadyRead() {
  QDataStream in(serverSocket_);
  in.setVersion(QDataStream::Qt_6_4);
  if (in.status() == QDataStream::Ok) {
    while (true) {
      if (!byteBlockSize_) {
        if (serverSocket_->bytesAvailable() < 2) {
          break;
        }
        in >> byteBlockSize_;
      }
      if (serverSocket_->bytesAvailable() < byteBlockSize_) {
        qDebug() << "Messenger::slotReadyRead() dead memory";
        break;
      }

      QString str;
      int type;
      uint sock;

      in >> type >> sock >> str;

      switch (type) {
        case DataType::Message: {
          byteBlockSize_ = 0;
          chatBox_->append(str);
          break;
        }
        case DataType::Authentication: {
          byteBlockSize_ = 0;
          qDebug() << "str: " << str;
          if (str.toInt() > 0) {
            emit successLoginSignal();
          } else {
            emit oppositeLoginSignal();
          }
          break;
        }
        case DataType::Registration: {
          byteBlockSize_ = 0;
          if (str == "OK") {
            emit successRegistrationSignal();
          } else if (str == "BAD") {
            emit oppositeRegistrationSignal();
          }
          break;
        }
        case DataType::HistoryLoading: {
          byteBlockSize_ = 0;
          chatBox_->append(str);
          break;
        }
        default:
          qDebug() << "Unknown message type";
          break;
      }
      break;
    }
  } else {
    qDebug() << "Read error";
  }

  qDebug() << serverSocket_->socketDescriptor() << "client socket send";
}

void Messenger::sendToServer(const QString& str) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(0) << clientData_->username
      << targetUser_->toPlainText() << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void Messenger::onSendButtonClicked() {
  sendToServer(messageBox_->toPlainText());
  messageBox_->clear();
}

void Messenger::onFindUserButtonClicked() {
  chatBox_->clear();
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(3) << clientData_->username
      << targetUser_->toPlainText();
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void Messenger::loginSlot(const QString& nickname,
                              const QString& password) {
  byteData_.clear();

  clientData_->username = nickname;

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(1) << nickname << password;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void Messenger::registrationSlot(const QString& nickname,
                                     const QString& password) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(2) << nickname << password;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void Messenger::setUsername() {
  nicknameLabel_->setText(clientData_->username);
}
