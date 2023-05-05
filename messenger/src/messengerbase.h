#ifndef MESSENGERBASE_H
#define MESSENGERBASE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MessengerBase : public QWidget
{
Q_OBJECT

public:
    MessengerBase(QWidget *parent = nullptr);
    ~MessengerBase();

private:
    QLabel* NicknameLabel;
    QTextEdit* chatBox;
    QTextEdit* messageBox;
    QPushButton* sendButton;
    QVBoxLayout* chatLayout;
    QHBoxLayout* messageLayout;
    QVBoxLayout* mainLayout;
};
#endif // MESSENGERBASE_H