/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once
#include <QLineEdit>
#include "llamaclient.h"

class ChatBoxOnModel : public QLineEdit {
    Q_OBJECT

public:
    ChatBoxOnModel(QLineEdit *parent = nullptr);

    void updateWindowLocation(int f_x, int f_y, int f_w, int f_h);

    void setLineStyle();

    void on_sendMsg();

    void onTextGenerated(const QString &text, const int &id);

    void onErrorOccurred(const QString &error, const int &id);

private:
    int ai_id = 1;
};
