#ifndef MESSENGERBASE_H
#define MESSENGERBASE_H

#include <QWidget>

class MessengerBase : public QWidget
{
    Q_OBJECT

public:
    MessengerBase(QWidget *parent = nullptr);
    ~MessengerBase();
};
#endif // MESSENGERBASE_H
