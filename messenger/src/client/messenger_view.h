#ifndef MESSENGERBASE_H
#define MESSENGERBASE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "client_data.h"

class MessengerView : public QWidget {
  Q_OBJECT

 public:
  explicit MessengerView(QWidget *parent = nullptr);
  ~MessengerView();

  void setUsername();

 private:
  Client *clientData_;

  QTcpSocket *serverSocket_;
  QByteArray byteData_;
  quint16 byteBlockSize_;
  void sendToServer(const QString &str);

  QLabel *nicknameLabel;
  QTextEdit *chatBox;
  QTextEdit *messageBox;
  QTextEdit *targetUser;
  QPushButton *sendButton;
  QPushButton *findUserButton;
  QVBoxLayout *chatLayout;
  QHBoxLayout *messageLayout;
  QHBoxLayout *findUserLayout;
  QVBoxLayout *mainLayout;

 public slots:
  void slotReadyRead();
  void loginSlot(const QString &nickname, const QString &password);
  void registrationSlot(const QString &nickname, const QString &password);

 private slots:
  void onSendButtonClicked();

 signals:
  void successLoginSignal();
  void successRegistrationSignal();
  void oppositeLoginSignal();
  void oppositeRegistrationSignal();
};
#endif  // MESSENGERBASE_H