/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#ifndef CHAT_H
#define CHAT_H
#include <QWidget>
#include "ollamaclient.h"

namespace Ui {
    class chat;
}

class ChatWidget : public QWidget {
    Q_OBJECT

public:
    OllamaClient client;

    void addMessage(const QString &message, const bool &isAI); // type must be 0 or 1

    explicit ChatWidget(QWidget *parent = nullptr);

    ~ChatWidget();

private slots:
    void on_sendMsg();

    void onTextGenerated(const QString &text);


    void onErrorOccurred(const QString &error);

private:
    Ui::chat *ui;
    int currentY;
    int standardHeight;
};


#endif //CHAT_H
