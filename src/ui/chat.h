/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
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

    void retranslateUI();

    void on_sendMsg();

    void onTextGenerated(const QString &text);


    void onErrorOccurred(const QString &error);

private:

    Ui::chat *ui;
    int currentY;
    int standardHeight;
};


#endif //CHAT_H
