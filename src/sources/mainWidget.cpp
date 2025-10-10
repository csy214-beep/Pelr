/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#include "mainWidget.h"
#include "ui_mainWidget.h"
#include  <QCloseEvent>
#include  <QFile>
#include  <QIcon>
#include  <QDebug>

mainWidget::mainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::mainWidget) {
    ui->setupUi(this);
    initUI();
}

void mainWidget::initUI() {
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    this->setWindowIcon(QIcon(":/assets/image/PLauncher.png"));
    this->setWindowTitle("PLauncher");
    this->setMaximumSize(this->minimumSize());
    QFile styleFile(":/assets/style/flutter.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
        styleFile.close();
    }
    Widget_chat = new ChatWidget(this);
    Widget_ManageStart = new ManageStartWidget(this);
    Widget_Setting = new SettingWidget(this);
    Widget_Todo = new todoWidget(this);
    Widget_chat->setGeometry(point_x, point_y, Widget_chat->width(), Widget_chat->height());
    Widget_ManageStart->setGeometry(point_x, point_y, Widget_ManageStart->width(), Widget_ManageStart->height());
    Widget_Setting->setGeometry(point_x, point_y, Widget_Setting->width(), Widget_Setting->height());
    Widget_Todo->setGeometry(point_x, point_y, Widget_Todo->width(), Widget_Todo->height());

    connect(ui->pushButton_4, &QPushButton::clicked, [=]() { switch_widget(Widget_Todo); });
    connect(ui->pushButton_3, &QPushButton::clicked, [=]() { switch_widget(Widget_chat); });
    connect(ui->pushButton_2, &QPushButton::clicked, [=]() { switch_widget(Widget_ManageStart); });
    connect(ui->pushButton, &QPushButton::clicked, [=]() { switch_widget(Widget_Setting); });
    // QSize size(72, 72);
    // ui->pushButton_3->setIconSize(size);
    // ui->pushButton_3->setIcon(QIcon(":/assets/Mono Icons/mi-message.png"));
    // ui->pushButton_2->setIconSize(size);
    // ui->pushButton_2->setIcon(QIcon(":/assets/Mono Icons/mi-three-rows.png"));
    // ui->pushButton->setIconSize(size);
    // ui->pushButton->setIcon(QIcon(":/assets/Mono Icons/mi-settings.png"));
    hideAllWidget();
    // 初始显示Chat界面
    Widget_chat->show();
}

void mainWidget::hideAllWidget() {
    Widget_chat->hide();
    Widget_ManageStart->hide();
    Widget_Setting->hide();
    Widget_Todo->hide();
}

void mainWidget::switch_widget(QWidget *w) {
    hideAllWidget();
    w->show();
}

mainWidget::~mainWidget() {
    delete ui;
}

void mainWidget::closeEvent(QCloseEvent *event) {
    if (!Widget_ManageStart->isSaved) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm",
                                      QString("您的更改尚未保存，是否要保存？"),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            Widget_ManageStart->saveMenuData();
        }
    }
    hide();
    event->ignore();
}

