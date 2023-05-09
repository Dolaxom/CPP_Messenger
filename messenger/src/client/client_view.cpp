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

  connect(messengerView, &MessengerView::oppositeLoginSignal, this,
          &ClientView::oppositeLoginSlot);

  connect(messengerView, &MessengerView::oppositeRegistrationSignal, this,
          &ClientView::oppositeRegistrationSlot);
}

void ClientView::successLoginSlot() {
  messengerView->setUsername();
  stackedWidget->setCurrentWidget(messengerView);
}

void ClientView::successRegistrationSlot() {
  authorizationView->setWarning("Successful registration! Now you can login!");
}

void ClientView::oppositeLoginSlot() {
  authorizationView->setWarning("Wrong username or password.");
}

void ClientView::oppositeRegistrationSlot() {
  authorizationView->setWarning("A user with the same name already exists.");
}
