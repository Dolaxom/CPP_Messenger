#include "authorization_view.h"

AuthorizationView::AuthorizationView(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  m_usernameEdit = new QLineEdit(this);
  m_passwordEdit = new QLineEdit(this);
  m_passwordEdit->setEchoMode(QLineEdit::Password);
  m_loginButton = new QPushButton("Войти", this);
  layout->addWidget(m_usernameEdit);
  layout->addWidget(m_passwordEdit);
  layout->addWidget(m_loginButton);

  connect(m_loginButton, &QPushButton::clicked, this,
          &AuthorizationView::tryAuthorization);
}

void AuthorizationView::tryAuthorization() { emit successfulLoginSignal(); }
