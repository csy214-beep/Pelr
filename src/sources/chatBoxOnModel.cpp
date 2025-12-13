/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "chatBoxOnModel.h"

#include <QCoreApplication>
#include <QApplication>
#include "data.hpp"
#include  <QDebug>
#include "BubbleBox.h"

ChatBoxOnModel::ChatBoxOnModel(QLineEdit *parent)
        : QLineEdit(parent) {
    // 窗口顶置，去标题栏，去除任务栏图标
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    if (DataManager::instance().getBasicData().isTop) {
        this->setWindowFlags(
                Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    } else {
        this->setWindowFlags(
                Qt::FramelessWindowHint | Qt::Tool);
    }
    //位置
    int _width = 300 * DataManager::instance().getBasicData().model_size / 150;
    int _height = 50 * DataManager::instance().getBasicData().model_size / 150;
    resize(_width, _height);
    //样式
    setLineStyle();
    //字体
    QFont font = DataManager::instance()._font;
    font.setPointSize((int) 18 * (DataManager::instance().getBasicData().model_size / 150));
    font.setWeight(50);
    // font.setBold(true);
    setFont(font);
    //信号槽
    connect(&client, &OllamaClient::textGenerated,
            this, &ChatBoxOnModel::onTextGenerated);
    connect(&client, &OllamaClient::errorOccurred,
            this, &ChatBoxOnModel::onErrorOccurred);
    connect(this, &ChatBoxOnModel::returnPressed, this, &ChatBoxOnModel::on_sendMsg);
}

void ChatBoxOnModel::updateWindowLocation(int f_x, int f_y, int f_w, int f_h) {
    QCoreApplication *coreapp = QCoreApplication::instance();
    QApplication *app = qobject_cast<QApplication *>(coreapp);
    if (!app)return;
    int x = f_x + f_w / 2 - width() / 2;
    int y = f_y + f_h + 20;
    // 位置未改变则不更新
    if (this->x() == x && this->y() == y)return;
    // 更新位置
    move(x, y);
}

void ChatBoxOnModel::on_sendMsg() {
    QString message = text();
    if (message.isEmpty()) return;
    clear();
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
    BubbleBox::instance()->setThinkingText();
}

void ChatBoxOnModel::onTextGenerated(const QString &text) {
    qDebug() << "text:" << text;
    // 处理生成的文本
    BubbleBox::instance()->textSet(text);
}

void ChatBoxOnModel::onErrorOccurred(const QString &error) {
    qDebug() << "error:" << error;
    // 处理错误
    BubbleBox::instance()->textSet("错误：" + error);
}

void ChatBoxOnModel::setLineStyle() {
    setStyleSheet(R"(
    QPushButton {
        background-color: rgba(13,126,191,128);
        color: white;
        border: none;
        padding: 8px;
        border-radius: 5px;
        font-weight: bold;
        min-width: 120px;
    }
    QPushButton:hover {
        background-color: rgba(13,126,191,255);
    }
    QPushButton:pressed {
        background-color: rgba(192,243,255,196);
    }
    QLineEdit {
        background-color: rgba(13,126,191,80);
        color: white;
        border: 2px solid rgba(13,126,191,160);
        padding: 8px 12px;
        border-radius: 5px;
        /*font-size: 14px;*/
        selection-background-color: rgba(192,243,255,196);
    }
    QLineEdit:hover {
        background-color: rgba(13,126,191,120);
        border: 2px solid rgba(13,126,191,200);
    }
    QLineEdit:focus {
        background-color: rgba(13,126,191,150);
        border: 2px solid rgba(192,243,255,255);
    }
    QLineEdit:disabled {
        background-color: rgba(128,128,128,80);
        color: rgba(255,255,255,128);
        border: 2px solid rgba(128,128,128,160);
    }
")");
}
