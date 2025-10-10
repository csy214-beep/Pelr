/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#include "KeyLabel.h"
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QVBoxLayout>
#include "adjustLabel.hpp"
#include  "data.hpp"

KeyLabelWidget::KeyLabelWidget(QWidget *parent)
    : QWidget(parent) {
    // 窗口顶置，去标题栏，去除任务栏图标，鼠标穿透，窗口透明，不可聚焦
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    if (DataManager::instance().getBasicData().isTop) {
        this->setWindowFlags(
            Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput |
            Qt::WindowDoesNotAcceptFocus);
    } else {
        this->setWindowFlags(
            Qt::FramelessWindowHint | Qt::Tool | Qt::WindowTransparentForInput |
            Qt::WindowDoesNotAcceptFocus);
    }

    this->keyLabel = new QLabel(this);
    this->mouseLabel = new QLabel(this);
    // 样式
    QString bg_color = DataManager::instance().getBasicData().color_keyLabel.first;
    QString text_color = DataManager::instance().getBasicData().color_keyLabel.second;
    bg_color = bg_color.isEmpty() ? "rgba(225, 128, 205, .8)" : bg_color;
    text_color = text_color.isEmpty() ? "rgba(255, 255, 255, .8)" : text_color;
    QString style = "background-color: " + bg_color + "; color: " + text_color + ";border-radius: 8px;";
    QFont font = DataManager::instance()._font;
    font.setPointSize((int) 18 * (DataManager::instance().getBasicData().model_size / 150));
    font.setWeight(50);
    font.setBold(true);
    keyLabel->setFont(font);
    mouseLabel->setFont(font);
    keyLabel->setStyleSheet(style);
    mouseLabel->setStyleSheet(style);
    keyLabel->setAlignment(Qt::AlignCenter);
    mouseLabel->setAlignment(Qt::AlignCenter);
    QList<QLabel *> labels;
    labels.append(keyLabel);
    labels.append(mouseLabel);
    adjustLabel(this, 0, 0, 1, 1, labels);

    keyLabel->hide();
    mouseLabel->hide();

    fadeTimer1 = new QTimer(this);
    fadeTimer2 = new QTimer(this);
    connect(fadeTimer1, &QTimer::timeout, this, [this]() { keyLabel->hide(); });
    connect(fadeTimer2, &QTimer::timeout, this, [this]() { mouseLabel->hide(); });
}

void KeyLabelWidget::on_keyRelease(QString keyName, QString modifiersName) {
    if (keyName.isEmpty()) {
        this->keyLabel->setText(modifiersName);
    } else if (modifiersName.isEmpty()) {
        this->keyLabel->setText(keyName);
    } else {
        this->keyLabel->setText(modifiersName + " " + keyName);
    }
    if (fadeTimer1->isActive()) {
        fadeTimer1->stop();
    }
    this->keyLabel->show();
    fadeTimer1->start(3000);
}

void KeyLabelWidget::on_mouseMove(int x, int y) {
    this->mouseLabel->setText("(" + QString::number(x) + "," + QString::number(y) + ")");
    if (fadeTimer2->isActive()) {
        fadeTimer2->stop();
    }
    mouseLabel->show();
    fadeTimer2->start(1000);
}

void KeyLabelWidget::updateWindowLocation(int f_x, int f_y, int f_w, int f_h) {
    QCoreApplication *coreapp = QCoreApplication::instance();
    QApplication *app = qobject_cast<QApplication *>(coreapp);
    if (!app)return;
    int x = f_x;
    int y = f_y + f_h / 1.6;
    // 位置未改变则不更新
    if (this->x() == x && this->y() == y)return;
    // 获取目标位置所在的屏幕
    QScreen *targetScreen = app->screenAt(QPoint(x, y));
    if (!targetScreen) {
        // 如果没有找到屏幕，使用主屏幕作为备选
        targetScreen = app->primaryScreen();
        if (!targetScreen) return;
    }
    // 获取屏幕的可用几何区域（考虑任务栏等系统UI）
    QRect screenGeometry = targetScreen->availableGeometry();
    // 窗口宽度
    int winWidth = width();
    int screenWidth = screenGeometry.width();
    int screenLeft = screenGeometry.left();

    // 水平边界检测
    if (x < screenLeft) {
        // 窗口左侧超出屏幕左边界
        x = x + f_w - winWidth;
    } else if (x + winWidth > screenLeft + screenWidth) {
        // 窗口右侧超出屏幕右边界
        if (winWidth <= screenWidth) {
            // 窗口宽度小于等于屏幕宽度，可以完全放入屏幕
            x = screenLeft + screenWidth - winWidth;
        } else {
            // 窗口宽度大于屏幕宽度，只能尽可能显示
            x = screenLeft;
        }
    }
    // 垂直边界检测
    if (y < screenGeometry.top()) {
        y = screenGeometry.top();
    } else if (y + height() > screenGeometry.bottom()) {
        y = screenGeometry.bottom() - height();
    }
    this->move(x, y);
}


KeyLabelWidget::~KeyLabelWidget() {
    delete fadeTimer1;
    delete fadeTimer2;
}

