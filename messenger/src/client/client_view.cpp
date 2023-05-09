/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#include "client_view.h"

ClientView::ClientView(QWidget* parent) : QMainWindow(parent) {
  stackedWidget = new QStackedWidget(this);

  authorizationView = new AuthorizationView(this);
  stackedWidget->addWidget(authorizationView);

  messengerView = new MessengerView(this);
  stackedWidget->addWidget(messengerView);

  setCentralWidget(stackedWidget);

  connect(authorizationView, &AuthorizationView::loginSignal, messengerView,
          &MessengerView::loginSlot);

  connect(authorizationView, &AuthorizationView::registationSignal,
          messengerView, &MessengerView::registrationSlot);

  connect(messengerView, &MessengerView::successLoginSignal, this,
          &ClientView::successLoginSlot);
  connect(messengerView, &MessengerView::successRegistrationSignal, this,
          &ClientView::successRegistrationSlot);
}
void ClientView::successLoginSlot() {
  stackedWidget->setCurrentWidget(messengerView);
}
void ClientView::successRegistrationSlot() {
  stackedWidget->setCurrentWidget(messengerView);
}
