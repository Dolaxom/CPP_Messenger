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

  connect(authorizationView, &AuthorizationView::successfulLoginSignal, this,
          &ClientView::successfulLoginSlot);
}

void ClientView::successfulLoginSlot() {
  stackedWidget->setCurrentWidget(messengerView);
}
