#include "messenger_view.h"

MessengerView::MessengerView(QWidget *parent) : QWidget(parent) {
  nicknameLabel = new QLabel("Nikolai");
  nicknameLabel->setAlignment(Qt::AlignCenter);

  chatBox = new QTextEdit();
  chatBox->setReadOnly(true);

  messageBox = new QTextEdit();
  messageBox->setFixedHeight(30);

  sendButton = new QPushButton("Send");
  connect(sendButton, &QPushButton::clicked, this,
          &MessengerView::onSendButtonClicked);

  chatLayout = new QVBoxLayout();
  chatLayout->addWidget(chatBox);

  messageLayout = new QHBoxLayout();
  messageLayout->addWidget(messageBox);
  messageLayout->addWidget(sendButton);

  mainLayout = new QVBoxLayout();
  mainLayout->addWidget(nicknameLabel);
  mainLayout->addLayout(chatLayout);
  mainLayout->addLayout(messageLayout);

  setLayout(mainLayout);

  socket_ = new QTcpSocket(this);

  connect(socket_, &QTcpSocket::readyRead, this, &MessengerView::slotReadyRead);
  connect(socket_, &QTcpSocket::disconnected, socket_,
          &QTcpSocket::deleteLater);

  socket_->connectToHost("127.0.0.1", 2323);

  byteBlockSize_ = 0;
}

void MessengerView::slotReadyRead() {  // Приём данных
  QDataStream in(socket_);
  in.setVersion(QDataStream::Qt_6_4);
  if (in.status() == QDataStream::Ok) {
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
      chatBox->append(str);
      break;
    }
  } else {
    chatBox->append("read error");
  }
}

void MessengerView::sendToServer(const QString str) {
  byteData_.clear();

  QDataStream out(&byteData_, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_4);
  out << quint16(0) << str;
  out.device()->seek(0);
  out << quint16(byteData_.size() - sizeof(quint16));
  socket_->write(byteData_);
}

void MessengerView::onSendButtonClicked() {
  sendToServer(messageBox->toPlainText());
  messageBox->clear();
}
