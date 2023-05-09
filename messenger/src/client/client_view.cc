/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#include "client_view.h"

ClientView::ClientView(QWidget* parent) : QMainWindow(parent) {
  stackedWidget_ = new QStackedWidget(this);

  authorizationView_ = new AuthorizationView(this);
  stackedWidget_->addWidget(authorizationView_);

  messengerView_ = new MessengerView(this);
  stackedWidget_->addWidget(messengerView_);

  setCentralWidget(stackedWidget_);

  connect(authorizationView_, &AuthorizationView::loginSignal, messengerView_,
          &MessengerView::loginSlot);

  connect(authorizationView_, &AuthorizationView::registationSignal,
          messengerView_, &MessengerView::registrationSlot);

  connect(messengerView_, &MessengerView::successLoginSignal, this,
          &ClientView::successLoginSlot);

  connect(messengerView_, &MessengerView::successRegistrationSignal, this,
          &ClientView::successRegistrationSlot);

  connect(messengerView_, &MessengerView::oppositeLoginSignal, this,
          &ClientView::oppositeLoginSlot);

  connect(messengerView_, &MessengerView::oppositeRegistrationSignal, this,
          &ClientView::oppositeRegistrationSlot);
}

void ClientView::successLoginSlot() {
  messengerView_->setUsername();
  stackedWidget_->setCurrentWidget(messengerView_);
}

void ClientView::successRegistrationSlot() {
  authorizationView_->setWarning("Successful registration! Now you can login!");
}

void ClientView::oppositeLoginSlot() {
  authorizationView_->setWarning("Wrong username or password.");
}

void ClientView::oppositeRegistrationSlot() {
  authorizationView_->setWarning("A user with the same name already exists.");
}
