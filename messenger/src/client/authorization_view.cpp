#include "authorization_view.h"

AuthorizationView::AuthorizationView(QWidget* parent) : QWidget(parent) {
  layout_ = new QFormLayout();
  usernameEdit_ = new QLineEdit();
  passwordEdit_ = new QLineEdit();
  passwordEdit_->setEchoMode(QLineEdit::Password);
  loginButton_ = new QPushButton("Войти");
  registerButton_ = new QPushButton("Зарегистрироваться");
  layout_->setLabelAlignment(Qt::AlignLeft);
  layout_->addRow("Имя пользователя:", usernameEdit_);
  layout_->addRow("Пароль:", passwordEdit_);
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  QSpacerItem* leftSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  QSpacerItem* rightSpacer =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
  buttonLayout->addItem(leftSpacer);
  buttonLayout->addWidget(loginButton_);
  buttonLayout->addWidget(registerButton_);
  buttonLayout->addItem(rightSpacer);
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addStretch();
  mainLayout->addLayout(layout_);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();
  mainLayout->setAlignment(Qt::AlignCenter);
  mainLayout->setContentsMargins(50, 50, 50, 50);
  setLayout(mainLayout);

  connect(loginButton_, &QPushButton::clicked, this,
          &AuthorizationView::tryAuthorization);

  connect(registerButton_, &QPushButton::clicked, this,
          &AuthorizationView::tryRegistration);
}

void AuthorizationView::tryAuthorization() {
  emit loginSignal(usernameEdit_->text(), passwordEdit_->text());
}
void AuthorizationView::tryRegistration() {
  emit registationSignal(usernameEdit_->text(), passwordEdit_->text());
}
