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


#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QProxyStyle>
#include <QFile>

class CustomMenuStyle : public QProxyStyle {
    Q_OBJECT

public:
    CustomMenuStyle() = default;

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override {
        if (element == PE_PanelMenu) {
            // 绘制圆角菜单背景 - 使用 QSS 中定义的背景色
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            // 获取 QSS 中定义的背景色，如果没有则使用默认白色
            QColor bgColor = widget->palette().color(QPalette::Window);
            if (bgColor == QColor(0, 0, 0, 0)) {
                bgColor = QColor(255, 255, 255);
            }

            // 绘制阴影
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(0, 0, 0, 15));
            painter->drawRoundedRect(option->rect.adjusted(0, 0, 0, 0), 8, 8);

            // 绘制背景
            painter->setBrush(bgColor);
            painter->drawRoundedRect(option->rect, 8, 8);

            // 绘制边框
            QColor borderColor = widget->palette().color(QPalette::WindowText);
            if (borderColor == QColor(0, 0, 0, 0)) {
                borderColor = QColor(224, 224, 224);
            }
            painter->setPen(QPen(borderColor, 1));
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(option->rect, 8, 8);

            painter->restore();
        } else {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }

    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const override {
        if (element == CE_MenuItem) {
            const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
            if (menuItem) {
                QRect rect = menuItem->rect;
                painter->save();

                // 绘制菜单项背景 - 让 QSS 处理颜色，我们只处理圆角
                if (menuItem->state & State_Selected && menuItem->state & State_Enabled) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Qt::NoBrush); // 不设置颜色，让 QSS 处理
                    painter->drawRoundedRect(rect.adjusted(2, 0, -2, 0), 4, 4);
                } else if (menuItem->state & State_MouseOver && menuItem->state & State_Enabled) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Qt::NoBrush); // 不设置颜色，让 QSS 处理
                    painter->drawRoundedRect(rect.adjusted(2, 0, -2, 0), 4, 4);
                }
                painter->restore();

                // 使用父类方法绘制图标、文本等
                QProxyStyle::drawControl(element, option, painter, widget);
            }
        } else {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override {
        switch (metric) {
            case PM_MenuPanelWidth:
                return 1;
            case PM_MenuHMargin:
                return 0;
            case PM_MenuVMargin:
                return 0;
            case PM_MenuButtonIndicator:
                return 16;
            default:
                return QProxyStyle::pixelMetric(metric, option, widget);
        }
    }

    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override {
        QSize result = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == CT_MenuItem) {
            result.setHeight(qMax(result.height(), 32));
        }
        return result;
    }
};

class CustomMenu : public QMenu {
    Q_OBJECT

public:
    explicit CustomMenu(QWidget *parent = nullptr);

    explicit CustomMenu(const QString &title, QWidget *parent = nullptr);

    void applyStyle();

private:
    void initStyle();

    CustomMenuStyle *m_style;
};
