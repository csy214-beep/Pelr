/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#pragma once

#include <QLabel>
#include <QMouseEvent>

class DoubleClickableLabel : public QLabel {
    Q_OBJECT // 必须包含 Q_OBJECT 宏才能使用信号

public:
    explicit DoubleClickableLabel(QWidget *parent = nullptr);

    explicit DoubleClickableLabel(const QString &text, QWidget *parent = nullptr);

signals:
    void doubleClicked(); // 声明双击信号

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // 重写双击事件
    // 可选：如果你想改变鼠标悬停样式，也可以重写 enterEvent/leaveEvent
    // void enterEvent(QEvent *event) override;
    // void leaveEvent(QEvent *event) override;
};

