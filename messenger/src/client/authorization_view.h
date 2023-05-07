#ifndef MESSENGER_AUTHORIZATION_VIEW_H
#define MESSENGER_AUTHORIZATION_VIEW_H

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class AuthorizationView : public QWidget {
  Q_OBJECT

 public:
  explicit AuthorizationView(QWidget* parent = nullptr);

  ~AuthorizationView() = default;

 private:
  QVBoxLayout* layout;
  QLineEdit* m_usernameEdit;
  QLineEdit* m_passwordEdit;
  QPushButton* m_loginButton;

  void tryAuthorization();

 signals:
  void successfulLoginSignal();
};

#endif  // MESSENGER_AUTHORIZATION_VIEW_H
