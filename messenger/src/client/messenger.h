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

class Messenger : public QWidget {
  Q_OBJECT

 public:
  explicit Messenger(QWidget *parent = nullptr);
  ~Messenger();

  void setUsername();

 private:
  Client *clientData_;

  QTcpSocket *serverSocket_;
  QByteArray byteData_;
  quint16 byteBlockSize_;
  void sendToServer(const QString &str);

  QLabel *nicknameLabel_;
  QTextEdit *chatBox_;
  QTextEdit *messageBox_;
  QTextEdit *targetUser_;
  QPushButton *sendButton_;
  QPushButton *findUserButton_;
  QVBoxLayout *chatLayout_;
  QHBoxLayout *messageLayout_;
  QHBoxLayout *findUserLayout_;
  QVBoxLayout *mainLayout_;

 public slots:
  void slotReadyRead();
  void loginSlot(const QString &nickname, const QString &password);
  void registrationSlot(const QString &nickname, const QString &password);

 private slots:
  void onSendButtonClicked();
  void onFindUserButtonClicked();  // find user and load messages history

 signals:
  void successLoginSignal();
  void successRegistrationSignal();
  void oppositeLoginSignal();
  void oppositeRegistrationSignal();
};
#endif  // MESSENGERBASE_H