#ifndef MESSENGER_AUTHORIZATION_H
#define MESSENGER_AUTHORIZATION_H

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Authorization : public QWidget {
  Q_OBJECT

 public:
  explicit Authorization(QWidget* parent = nullptr);

  ~Authorization() = default;

  void setWarning(const QString&& str);

 private:
  QLineEdit* usernameEdit_;
  QLineEdit* passwordEdit_;
  QPushButton* loginButton_;
  QPushButton* registerButton_;
  QFormLayout* layout_;
  QLabel* warningLabel_;

  void tryAuthorization();
  void tryRegistration();

 signals:
  void loginSignal(QString nickname, QString password);
  void registationSignal(QString nickname, QString password);
};

#endif  // MESSENGER_AUTHORIZATION_H
