/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once


#include <QWidget>
#include "chat.h"
#include "manageStart.h"
#include "setting.h"
#include  "todoWidget.h"

namespace Ui {
    class mainWidget;
}

class mainWidget : public QWidget {
    Q_OBJECT

public:
    ChatWidget *Widget_chat;
    ManageStartWidget *Widget_ManageStart;
    SettingWidget *Widget_Setting;
    todoWidget *Widget_Todo;
    int point_x = 130;
    int point_y = 30;

    explicit mainWidget(QWidget *parent = nullptr);

    ~mainWidget();

    void closeEvent(QCloseEvent *event) override;

    void initUI();
 void showEvent(QShowEvent *event) override;

private slots:
    void switch_widget(QWidget *w);

    void hideAllWidget();

private:
    Ui::mainWidget *ui;
};
