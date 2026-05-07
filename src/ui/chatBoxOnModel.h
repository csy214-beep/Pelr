
#pragma once
#include <QLineEdit>
#include "llamaclient.h"

class ChatBoxOnModel : public QLineEdit
{
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
