#include "authorization.h"

Authorization::Authorization(QWidget* parent) : QWidget(parent) {
  layout_ = new QFormLayout();
  warningLabel_ = new QLabel("");
  usernameEdit_ = new QLineEdit();
  passwordEdit_ = new QLineEdit();
  passwordEdit_->setEchoMode(QLineEdit::Password);
  loginButton_ = new QPushButton("Login");
  registerButton_ = new QPushButton("Signup");
  layout_->setLabelAlignment(Qt::AlignLeft);
  layout_->addRow("Username:", usernameEdit_);
  layout_->addRow("Password:", passwordEdit_);
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
  mainLayout->addWidget(warningLabel_);
  warningLabel_->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(layout_);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();
  mainLayout->setAlignment(Qt::AlignCenter);
  mainLayout->setContentsMargins(50, 50, 50, 50);
  setLayout(mainLayout);

  connect(loginButton_, &QPushButton::clicked, this,
          &Authorization::tryAuthorization);

  connect(registerButton_, &QPushButton::clicked, this,
          &Authorization::tryRegistration);
}

void Authorization::tryAuthorization() {
  emit loginSignal(usernameEdit_->text(), passwordEdit_->text());
}

void Authorization::tryRegistration() {
  emit registationSignal(usernameEdit_->text(), passwordEdit_->text());
}

void Authorization::setWarning(const QString&& str) {
  usernameEdit_->clear();
  passwordEdit_->clear();
  warningLabel_->setText(str);
}
