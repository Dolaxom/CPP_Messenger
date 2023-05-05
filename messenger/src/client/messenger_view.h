#ifndef MESSENGERBASE_H
#define MESSENGERBASE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MessengerView : public QWidget
{
Q_OBJECT

public:
    explicit MessengerView(QWidget *parent = nullptr);
    ~MessengerView() = default;

private:
    QLabel* nicknameLabel;
    QTextEdit* chatBox;
    QTextEdit* messageBox;
    QPushButton* sendButton;
    QVBoxLayout* chatLayout;
    QHBoxLayout* messageLayout;
    QVBoxLayout* mainLayout;
};
#endif // MESSENGERBASE_H