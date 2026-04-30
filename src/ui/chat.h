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
#include "llamaclient.h"

namespace Ui {
    class chat;
}

class ChatWidget : public QWidget {
Q_OBJECT

public:
    void addMessage(const QString &message, const bool &isAI); // type must be 0 or 1

    explicit ChatWidget(QWidget *parent = nullptr);

    ~ChatWidget();

private slots:

    void retranslateUI();

    void on_sendMsg();

    void onTextGenerated(const QString &text, const int &id);

    void onErrorOccurred(const QString &error, const int &id);

private:
    int ai_id = 2;
    Ui::chat *ui;
    int currentY;
    int standardHeight;
};


#endif //CHAT_H
