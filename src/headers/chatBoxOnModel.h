/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once
#include <QLineEdit>
#include "ollamaclient.h"

class ChatBoxOnModel : public QLineEdit {
    Q_OBJECT

public:
    OllamaClient client;

    ChatBoxOnModel(QLineEdit *parent = nullptr);

    void updateWindowLocation(int f_x, int f_y, int f_w, int f_h);

    void setLineStyle();

    void on_sendMsg();

    void onTextGenerated(const QString &text);

    void onErrorOccurred(const QString &error);
};
