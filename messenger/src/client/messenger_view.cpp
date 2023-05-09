#include "messenger_view.h"

MessengerView::MessengerView(QWidget* parent) : QWidget(parent) {
  clientData_ = new Client();

  nicknameLabel = new QLabel();
  nicknameLabel->setAlignment(Qt::AlignCenter);

  chatBox = new QTextEdit();
  chatBox->setReadOnly(true);

  messageBox = new QTextEdit();
  messageBox->setFixedHeight(30);

  findUserLayout = new QHBoxLayout();
  findUserButton = new QPushButton("Find");

  sendButton = new QPushButton("Send");
  connect(sendButton, &QPushButton::clicked, this,
          &MessengerView::onSendButtonClicked);

  QLabel* targetUserLabel = new QLabel("Enter target username:");
  targetUser = new QTextEdit();
  targetUser->setFixedHeight(30);

  chatLayout = new QVBoxLayout();
  findUserLayout->addWidget(targetUser);
  findUserLayout->addWidget(findUserButton);
  chatLayout->addWidget(targetUserLabel);
  chatLayout->addLayout(findUserLayout);
  chatLayout->addWidget(targetUser);
  chatLayout->addWidget(chatBox);

  messageLayout = new QHBoxLayout();
  messageLayout->addWidget(messageBox);
  messageLayout->addWidget(sendButton);

  mainLayout = new QVBoxLayout();
  mainLayout->addWidget(nicknameLabel);
  mainLayout->addLayout(chatLayout);
  mainLayout->addLayout(messageLayout);

  setLayout(mainLayout);

  serverSocket_ = new QTcpSocket(this);

  connect(serverSocket_, &QTcpSocket::readyRead, this,
          &MessengerView::slotReadyRead);
  connect(serverSocket_, &QTcpSocket::disconnected, serverSocket_,
          &QTcpSocket::deleteLater);

  serverSocket_->connectToHost("127.0.0.1", 2323);

  byteBlockSize_ = 0;
}

MessengerView::~MessengerView() { delete clientData_; }

void MessengerView::slotReadyRead() {  // Приём данных
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
        qDebug() << "MessengerView::slotReadyRead() dead memory";
        break;
      }

      QString str;
      int type;
      uint sock;

      in >> type >> sock >> str;

      if (!type) {
        qDebug() << "str: " << str;
        byteBlockSize_ = 0;
        chatBox->append(str);
      } else if (type == 1) {
        byteBlockSize_ = 0;
        qDebug() << "str: " << str;
        if (str.toInt() > 0) {
          emit successLoginSignal();
        } else {
          emit oppositeLoginSignal();
        }
      } else if (type == 2) {
        byteBlockSize_ = 0;
        if (str == "OK") {
          emit successRegistrationSignal();
        } else if (str == "BAD") {
          emit oppositeRegistrationSignal();
        }
      }

      break;
    }
  } else {
    qDebug() << "Read error";
  }

  qDebug() << serverSocket_->socketDescriptor() << "client socket send";
}

void MessengerView::sendToServer(const QString& str) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(0) << targetUser->toPlainText().toUInt() << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void MessengerView::onSendButtonClicked() {
  sendToServer(messageBox->toPlainText());
  messageBox->clear();
}

void MessengerView::loginSlot(const QString& nickname,
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

void MessengerView::registrationSlot(const QString& nickname,
                                     const QString& password) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << int(2) << nickname << password;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  serverSocket_->write(byteData_);
}

void MessengerView::setUsername() {
  nicknameLabel->setText(clientData_->username);
}
