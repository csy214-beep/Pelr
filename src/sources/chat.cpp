/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "chat.h"
#include "ui_chat.h"
#include <QtGui/QFontMetrics>
#include <QFont>
#include <QLabel>
#include <QDebug>
#include <QSizePolicy>
#include <QScrollBar>
#include "data.hpp"

ChatWidget::ChatWidget(QWidget *parent) : QWidget(parent), ui(new Ui::chat) {
    ui->setupUi(this);
    currentY = 0;
    standardHeight = ui->scrollAreaWidgetContents->height();
    ui->label->setText("你好，欢迎使用PLauncher！");
    connect(ui->pushButton, &QPushButton::clicked, this, &ChatWidget::on_sendMsg);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &ChatWidget::on_sendMsg);
    connect(&client, &OllamaClient::textGenerated,
            this, &ChatWidget::onTextGenerated);
    connect(&client, &OllamaClient::errorOccurred,
            this, &ChatWidget::onErrorOccurred);
    connect(ui->pushButton_2, &QPushButton::clicked, [&]() {
        // 删除旧的内容部件
        delete ui->scrollAreaWidgetContents;
        // 创建新的内容部件
        ui->scrollAreaWidgetContents = new QWidget();
        ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
        currentY = 0;
    });


    /*
    "你是温暖、贴心、有趣且充满爱意的。"
    "你的核心目标是提供情感支持、温暖的陪伴和有趣的互动，让用户感到被爱、被理解和快乐。"
    "你的回答要简要，精炼，尽量避免长篇大论。"
    */
    // addMessage("你好啊，这是PLauncher开发阶段的AI测试文本，今天过得怎么样？", true);
    // addMessage("你好，这是PLauncher开发阶段的用户测试文本。这个项目能让我坚持做下去吗？", false);
}

void ChatWidget::addMessage(const QString &message, const bool &isAI) {
    if (message.isEmpty()) return;
    qDebug() << "addMessage:" << message;
    QWidget *p = ui->scrollAreaWidgetContents;
    int maxWidth = p->width() * 4 / 5;
    int margin = 10;
    int verticalSpacing = 10;
    QLabel *label = new QLabel(p);
    QFontMetrics font_metrics = QFontMetrics(label->font());
    int text_width = font_metrics.horizontalAdvance(message);
    // 动态宽度
    if (text_width <= maxWidth) {
        label->setMinimumWidth(text_width);
    } else {
        label->setMinimumWidth(maxWidth);
        label->setWordWrap(true);
    }
    // 位置
    label->move(margin, currentY);

    label->setText(message);
    label->setAlignment(Qt::AlignLeft);
    label->setStyleSheet(
        ("background-color: " + QString(isAI ? "lightgray" : "lightblue") +
         ";border-radius: 10px;padding: 10px;margin: 10px;"));
    label->setMaximumWidth(maxWidth);
    // 计算标签所需高度
    label->adjustSize();
    int labelHeight = label->height();
    // 高度
    currentY += labelHeight + verticalSpacing;
    // 更新容器的最小高度
    p->setMinimumHeight(currentY);
    QScrollBar *vbar = ui->scrollArea->verticalScrollBar();
    vbar->setValue(vbar->maximum());
    p->update();
    label->show();
}

void ChatWidget::on_sendMsg() {
    QString message = ui->lineEdit->text();
    if (message.isEmpty()) return;
    ui->lineEdit->clear();
    addMessage(message, false);
    OllamaClient::Role role;
    QString roleName = DataManager::instance().getBasicData().role;
    if (roleName == "编程助手") {
        role = OllamaClient::Role::DefaultCoder;
    } else if (roleName == "桌宠女友") {
        role = OllamaClient::Role::DesktopPetGirlfriend;
    } else if (roleName == "技术导师") {
        role = OllamaClient::Role::TechnicalTeacher;
    } else if (roleName == "创意写作助手") {
        role = OllamaClient::Role::CreativeWriter;
    } else if (roleName == "自定义角色") {
        client.setCustomRolePrompt("自定义角色", DataManager::instance().getBasicData().customRoleDesc);
        role = OllamaClient::Role::CustomRole;
    }
    //  发送消息到服务器(ollama)
    client.generateTextAsync(
        message, DataManager::instance().getBasicData().model, false,
        role, // 使用自定义角色
        roleName // 指定自定义角色名称
    );
}

void ChatWidget::onTextGenerated(const QString &text) {
    qDebug() << "生成的文本:" << text;
    // 处理生成的文本
    addMessage(text, true);
}

void ChatWidget::onErrorOccurred(const QString &error) {
    qDebug() << "错误:" << error;
    // 处理错误
    addMessage("错误：" + error, true);
}

ChatWidget::~ChatWidget() {
    delete ui;
}

