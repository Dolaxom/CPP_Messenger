/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#ifndef MESSENGER_VIEW_H
#define MESSENGER_VIEW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "authorization.h"
#include "messenger.h"

class Client : public QMainWindow {
  Q_OBJECT

 public:
  Client(QWidget* parent = nullptr);
  ~Client() = default;

 private:
  QStackedWidget* stackedWidget_;

  Messenger* messengerView_;
  Authorization* authorizationView_;

 private slots:
  void successLoginSlot();
  void successRegistrationSlot();
  void oppositeLoginSlot();
  void oppositeRegistrationSlot();
};
#endif  // MESSENGER_VIEW_H
