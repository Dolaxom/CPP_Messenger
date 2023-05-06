#ifndef MESSENGERBASE_H
#define MESSENGERBASE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class MessengerView : public QWidget {
  Q_OBJECT

 public:
  explicit MessengerView(QWidget *parent = nullptr);
  ~MessengerView() = default;

 private:
  QTcpSocket *socket_;
  QByteArray byteData_;
  void sendToServer(const QString str);
  quint16 byteBlockSize_;

  QLabel *nicknameLabel;
  QTextEdit *chatBox;
  QTextEdit *messageBox;
  QPushButton *sendButton;
  QVBoxLayout *chatLayout;
  QHBoxLayout *messageLayout;
  QVBoxLayout *mainLayout;

 public slots:
  void slotReadyRead();

 private slots:
  void onSendButtonClicked();
};
#endif  // MESSENGERBASE_H