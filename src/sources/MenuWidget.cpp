/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "MenuWidget.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QMouseEvent>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include <QStyle>
#include <QPainter>
#include <QDebug>
#include <QFocusEvent>
#include "data.hpp"

#include "data.hpp"

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
    applyStyles();

    // 初始化隐藏计时器
    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    connect(m_hideTimer, &QTimer::timeout, this, &MenuWidget::hideMenu);

    // 安装事件过滤器到应用程序，监控全局焦点变化
    qApp->installEventFilter(this);

    //置顶
    if (DataManager::instance().getBasicData().isTop) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    }
    // 设置窗口背景和圆角
    setAttribute(Qt::WA_TranslucentBackground);

    // 启用鼠标跟踪
    setMouseTracking(true);
}

MenuWidget::~MenuWidget() {
    // 清理事件过滤器
    qApp->removeEventFilter(this);
}

void MenuWidget::showNearMouse() {
    // 获取鼠标当前位置
    QPoint mousePos = QCursor::pos();

    // 获取屏幕尺寸
    QScreen *screen = QApplication::screenAt(mousePos);
    QRect screenGeometry = screen->geometry();

    //距离鼠标的距离
    int distance = 60 * DataManager::instance().getBasicData().model_size / 150;

    // 获取窗口尺寸
    adjustSize();
    int width = this->width();
    int height = this->height();

    // 计算显示位置 - 优先显示在鼠标右侧
    int x = mousePos.x() + distance;
    int y = mousePos.y();

    // 检查右侧空间是否足够
    if (x + width > screenGeometry.right()) {
        // 右侧空间不足，显示在左侧
        x = mousePos.x() - width - distance;
    }

    // 检查底部空间是否足够
    if (y + height > screenGeometry.bottom()) {
        y = screenGeometry.bottom() - height;
    }

    // 确保窗口不会超出屏幕顶部
    if (y < screenGeometry.top()) {
        y = screenGeometry.top();
    }

    // 确保窗口不会超出屏幕左侧
    if (x < screenGeometry.left()) {
        x = screenGeometry.left();
    }
    qInfo() << "Menu show at: (" << x << "," << y << ")";

    // 移动窗口到计算位置
    move(x, y);

    // 显示窗口
    show();

    // 激活窗口
    activateWindow();

    // 启动隐藏计时器（默认5秒）
    startHideTimer(short_interval);
}

void MenuWidget::hideMenu() {
    qInfo() << "Auto hiding menu";
    hide();
    stopHideTimer();
}

void MenuWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    // 绘制圆角矩形背景
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(45, 45, 48, 192));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 10, 10);
}

void MenuWidget::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 标题
    QLabel *titleLabel = new QLabel("PLauncher");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);
}

void MenuWidget::applyStyles() {
    setStyleSheet(R"(
            menuWidget {
                background: transparent;
            }
            QLabel#titleLabel {
                color: #e6e6e6;
                font-weight: bold;
                font-size: 14px;
                padding: 5px;
            }
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
        )");
}

// 鼠标进入窗口 (Qt5版本)
void MenuWidget::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    m_mouseInWindow = true;
    stopHideTimer();  // 鼠标在窗口内时停止自动隐藏
    qDebug() << "Mouse entered menu, hiding timer stopped";
}

// 鼠标离开窗口
void MenuWidget::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    m_mouseInWindow = false;

    // 鼠标离开但窗口仍有焦点时，使用较长超时
    if (hasFocus()) {
        restartHideTimer(long_interval);  // 8秒后隐藏
    } else {
        restartHideTimer(short_interval);  // 失去焦点时5秒后隐藏
    }
    qDebug() << "Mouse left menu, hiding timer restarted";
}

// 获得焦点
void MenuWidget::focusInEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    qDebug() << "Menu gained focus";

    // 获得焦点时，如果鼠标不在窗口内，使用正常超时
    if (!m_mouseInWindow) {
        restartHideTimer(long_interval);
    }
    // 如果鼠标在窗口内，定时器已经停止，不需要操作
}


// 失去焦点
void MenuWidget::focusOutEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    qDebug() << "Menu lost focus";

    // 检查新的焦点控件是否在本窗口层级内
    if (!isFocusInWindowHierarchy()) {
        // 焦点完全离开窗口层级，使用较短超时
        restartHideTimer(short_interval);
    }
    // 如果焦点还在窗口层级内（比如切换到子控件），不改变定时器状态
}

// 事件过滤器 - 监控全局焦点变化
bool MenuWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::WindowDeactivate) {
        // 整个应用程序失去激活状态时立即隐藏
        if (qApp->activeWindow() == nullptr) {
            qDebug() << "Application deactivated, hiding menu immediately";
            hideMenu();
        }
    }
    return QWidget::eventFilter(obj, event);
}

// 检查焦点是否在窗口层级内
bool MenuWidget::isFocusInWindowHierarchy() const {
    QWidget *focused = QApplication::focusWidget();
    if (!focused) return false;

    // 检查焦点控件是否是这个窗口或其子控件
    return isAncestorOf(focused) || focused == this;
}

// 启动隐藏计时器
void MenuWidget::startHideTimer(int timeoutMs) {
    if (m_hideTimer->isActive()) {
        m_hideTimer->stop();
    }
    m_hideTimer->start(timeoutMs);
    qDebug() << "Hide timer started:" << timeoutMs << "ms";
}

// 停止隐藏计时器
void MenuWidget::stopHideTimer() {
    if (m_hideTimer->isActive()) {
        m_hideTimer->stop();
        qDebug() << "Hide timer stopped";
    }
}

// 重启隐藏计时器
void MenuWidget::restartHideTimer(int timeoutMs) {
    stopHideTimer();
    startHideTimer(timeoutMs);
}

void MenuWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        left_button_down = true;
        dragStartPos = event->globalPos() - frameGeometry().topLeft();
    }
}

void MenuWidget::mouseMoveEvent(QMouseEvent *event) {
    if (left_button_down && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragStartPos);
    }
}

void MenuWidget::mouseReleaseEvent(QMouseEvent *event) {
    // 按住鼠标释放时停止拖动
    if (event->button() == Qt::LeftButton) {
        left_button_down = false;
    }
}

void MenuWidget::closeEvent(QCloseEvent *event) {
    hide();
    stopHideTimer();
    event->ignore();
}