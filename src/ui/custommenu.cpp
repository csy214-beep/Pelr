/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "custommenu.h"

CustomMenu::CustomMenu(QWidget *parent)
    : QMenu(parent), m_style(new CustomMenuStyle()) {
    initStyle();
}

CustomMenu::CustomMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent), m_style(new CustomMenuStyle()) {
    initStyle();
}

void CustomMenu::initStyle() {
    // 设置自定义样式
    setStyle(m_style);

    // 设置无边框和透明背景，确保圆角正确显示
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 设置基础样式表，确保与你的 QSS 兼容
    /*
    setStyleSheet(R"(
        QMenu {
            background-color: transparent;
            border: none;
            padding: 4px 0;
            margin: 2px;
        }

        QMenu::item {
            background-color: transparent;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            margin: 1px 4px;
            color: #2d3748;
            font-size: 13px;
            font-weight: 500;
            min-width: 120px;
        }

        QMenu::item:disabled {
            color: #9e9e9e;
        }

        QMenu::item:selected {
            background-color: transparent;
            color: white;
        }

        QMenu::item:hover:!selected {
            background-color: transparent;
            color: #2196f3;
        }

        QMenu::separator {
            height: 1px;
            background: #e2e8f0;
            margin: 3px 8px;
        }

        QMenu::icon {
            margin-left: 4px;
        }

        QMenu::indicator {
            width: 16px;
            height: 16px;
            margin-right: 4px;
        }

        QMenu::right-arrow {
            image: none;
            width: 16px;
            height: 16px;
        }
    )");
    */
    applyStyle();
}

void CustomMenu::applyStyle() {
    QFile styleFile(":/assets/style/menu.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}
