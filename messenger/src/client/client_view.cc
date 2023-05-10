/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#include "client_view.h"

ClientView::ClientView(QWidget* parent) : QMainWindow(parent) {
  stackedWidget_ = new QStackedWidget(this);

  authorizationView_ = new Authorization(this);
  stackedWidget_->addWidget(authorizationView_);

  messengerView_ = new Messenger(this);
  stackedWidget_->addWidget(messengerView_);

  setCentralWidget(stackedWidget_);

  connect(authorizationView_, &Authorization::loginSignal, messengerView_,
          &Messenger::loginSlot);

  connect(authorizationView_, &Authorization::registationSignal,
          messengerView_, &Messenger::registrationSlot);

  connect(messengerView_, &Messenger::successLoginSignal, this,
          &ClientView::successLoginSlot);

  connect(messengerView_, &Messenger::successRegistrationSignal, this,
          &ClientView::successRegistrationSlot);

  connect(messengerView_, &Messenger::oppositeLoginSignal, this,
          &ClientView::oppositeLoginSlot);

  connect(messengerView_, &Messenger::oppositeRegistrationSignal, this,
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
