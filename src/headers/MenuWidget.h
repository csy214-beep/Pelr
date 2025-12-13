/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QScreen>
#include <QMouseEvent>
#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QVBoxLayout>
#include <QFocusEvent>

class MenuWidget : public QWidget {
Q_OBJECT

public:
    QVBoxLayout *mainLayout;

    MenuWidget(QWidget *parent = nullptr);

    ~MenuWidget();

    void showNearMouse();

public slots:

    void hideMenu();

protected:
    void paintEvent(QPaintEvent *event) override;

    void enterEvent(QEvent *event) override;        // Qt5: 鼠标进入
    void leaveEvent(QEvent *event) override;        // 鼠标离开
    void focusInEvent(QFocusEvent *event) override;    // 获得焦点
    void focusOutEvent(QFocusEvent *event) override;   // 失去焦点
    bool eventFilter(QObject *obj, QEvent *event) override;  // 事件过滤器

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    int long_interval = 8000;  // 长间隔
    int short_interval = 5000;   // 短间隔
    void setupUI();

    void applyStyles();

    void startHideTimer(int timeoutMs = 5000);  // 启动隐藏计时器
    void stopHideTimer();                       // 停止隐藏计时器
    void restartHideTimer(int timeoutMs = 5000); // 重启隐藏计时器
    bool isFocusInWindowHierarchy() const;      // 检查焦点是否在窗口层级内

    bool left_button_down = false;
    QPoint dragStartPos;

    // 自动隐藏相关
    QTimer *m_hideTimer;
    bool m_mouseInWindow = false;  // 鼠标是否在窗口内
};