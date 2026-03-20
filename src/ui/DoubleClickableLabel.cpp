/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "DoubleClickableLabel.h"

DoubleClickableLabel::DoubleClickableLabel(QWidget *parent)
    : QLabel(parent) {
    // 设置 QLabel 为可接受鼠标事件 (通常默认就是，但明确设置更安全)
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    // 可选：设置鼠标悬停时显示手型光标，提示用户可点击
    setCursor(Qt::PointingHandCursor);
}

DoubleClickableLabel::DoubleClickableLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setCursor(Qt::PointingHandCursor);
}

void DoubleClickableLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    // 检查是否是左键双击 (可选，根据需求调整)
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发射自定义信号
    }
    // 调用父类实现，确保其他默认行为（如果有的话）仍然执行
    QLabel::mouseDoubleClickEvent(event);
}

// 如果你重写了 enterEvent/leaveEvent 来改变光标，记得调用父类
/*
void DoubleClickableLabel::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QLabel::enterEvent(event);
}

void DoubleClickableLabel::leaveEvent(QEvent *event)
{
    unsetCursor(); // 或者设置回默认光标
    QLabel::leaveEvent(event);
}
*/
