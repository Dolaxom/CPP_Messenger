/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#ifndef MESSENGER_VIEW_H
#define MESSENGER_VIEW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "authorization_view.h"
#include "messenger_view.h"

class ClientView : public QMainWindow {
  Q_OBJECT

 public:
  ClientView(QWidget* parent = nullptr);
  ~ClientView() = default;

 private:
  QStackedWidget* stackedWidget;

  MessengerView* messengerView;
  AuthorizationView* authorizationView;

 public slots:
  void successfulLoginSlot();
};
#endif  // MESSENGER_VIEW_H
