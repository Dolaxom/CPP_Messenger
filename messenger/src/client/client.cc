/*
 * THIS IS THE MAIN UI FILE FOR THE CLIENT APP
 */

#include "client.h"

Client::Client(QWidget* parent) : QMainWindow(parent) {
  stackedWidget_ = new QStackedWidget(this);

  authorizationView_ = new Authorization(this);
  stackedWidget_->addWidget(authorizationView_);

  messengerView_ = new Messenger(this);
  stackedWidget_->addWidget(messengerView_);

  setCentralWidget(stackedWidget_);

  connect(authorizationView_, &Authorization::loginSignal, messengerView_,
          &Messenger::loginSlot);

  connect(authorizationView_, &Authorization::registationSignal, messengerView_,
          &Messenger::registrationSlot);

  connect(messengerView_, &Messenger::successLoginSignal, this,
          &Client::successLoginSlot);

  connect(messengerView_, &Messenger::successRegistrationSignal, this,
          &Client::successRegistrationSlot);

  connect(messengerView_, &Messenger::oppositeLoginSignal, this,
          &Client::oppositeLoginSlot);

  connect(messengerView_, &Messenger::oppositeRegistrationSignal, this,
          &Client::oppositeRegistrationSlot);
}

void Client::successLoginSlot() {
  messengerView_->setUsername();
  stackedWidget_->setCurrentWidget(messengerView_);
}

void Client::successRegistrationSlot() {
  authorizationView_->setWarning("Successful registration! Now you can login!");
}

void Client::oppositeLoginSlot() {
  authorizationView_->setWarning("Wrong username or password.");
}

void Client::oppositeRegistrationSlot() {
  authorizationView_->setWarning("A user with the same name already exists.");
}
