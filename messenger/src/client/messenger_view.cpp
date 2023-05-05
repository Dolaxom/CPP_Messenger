#include "messenger_view.h"

MessengerView::MessengerView(QWidget *parent)
        : QWidget(parent) {
    nicknameLabel = new QLabel("Nikolai");
    nicknameLabel->setAlignment(Qt::AlignCenter);

    chatBox = new QTextEdit();
    chatBox->setReadOnly(true);

    messageBox = new QTextEdit();
    messageBox->setFixedHeight(30);

    sendButton = new QPushButton("Send");

    chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatBox);

    messageLayout = new QHBoxLayout();
    messageLayout->addWidget(messageBox);
    messageLayout->addWidget(sendButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(nicknameLabel);
    mainLayout->addLayout(chatLayout);
    mainLayout->addLayout(messageLayout);

    setLayout(mainLayout);
}