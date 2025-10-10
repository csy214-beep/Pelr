#pragma once
#include <QLineEdit>
#include "ollamaclient.h"
/*
PROJECT_NAME PLauncher
PRODUCT_NAME CLion
NAME chatBoxOnModel
AUTHOR Pfolg
TIME 2025/9/22 17:04
*/
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
