/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "tray.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QFile>
#include "launcher.hpp"
#include "data.hpp"
#include "launcherMenu.hpp"
#include "custommenu.h"
// 初始化静态成员变量
TrayIcon *TrayIcon::m_instance = nullptr;

TrayIcon *TrayIcon::instance() {
    if (!m_instance) {
        m_instance = new TrayIcon();
    }
    return m_instance;
}

void TrayIcon::showMessage(const QString &title, const QString &msg,
                           QSystemTrayIcon::MessageIcon icon, int timeout) {
    if (m_instance && !m_instance->m_silentMode) {
        m_instance->QSystemTrayIcon::showMessage(title, msg, icon, timeout);
    }
}

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(QIcon(":/assets/image/PLauncher.png"), parent) {
    setToolTip(
        DataManager::instance().const_config_data.name + " "
        + DataManager::instance().const_config_data.version
    );

    // 创建右键菜单
    menu = new CustomMenu();

    action_resetWinLoc = new QAction("重置位置", this);
    action_showWin = new QAction("显示界面", this);
    action_silentMode = new QAction("静默模式", this);
    action_switchDrag = new QAction("锁定位置", this);
    action_switchTransparent = new QAction("鼠标穿透", this);
    action_mediaPlayer = new QAction("播放媒体", this);
    QAction *action_openPath = new QAction("打开目录", this);
    QAction *exitAction = new QAction("退出程序", this);

    // 连接信号和槽（保持原有连接不变）
    connect(exitAction, &QAction::triggered, []() {
        QCoreApplication::quit();
    });

    connect(action_openPath, &QAction::triggered, []() {
        QString appDir = QCoreApplication::applicationDirPath();
        launchByPath(appDir);
    });

    connect(action_silentMode, &QAction::triggered, [this]() {
        m_silentMode = !m_silentMode;
        switchText(action_silentMode);
    });

    connect(action_switchDrag, &QAction::triggered, [this]() {
        switchText(action_switchDrag);
    });

    connect(action_switchTransparent, &QAction::triggered, [this]() {
        switchText(action_switchTransparent);
    });

    QAction *action_startApp = new QAction("启动项目", this);
    action_startApp->setMenu(launcherMenu::instance());
    // 添加菜单项到菜单
    menu->addAction(action_startApp);
    menu->addSeparator();

    menu->addActions({
        action_resetWinLoc, action_silentMode, action_switchDrag,
        action_showWin, action_mediaPlayer, action_openPath
    });
    menu->addSeparator();
    menu->addAction(exitAction);

    // 设置托盘图标的菜单
    this->setContextMenu(menu);

    // 显示托盘图标
    this->show();

    qDebug() << "TrayIcon singleton initialized";
}

void TrayIcon::switchText(QAction *action) {
    QString text = action->text();
    if (text.contains("* ")) {
        action->setText(text.replace("* ", ""));
    } else {
        action->setText("* " + text);
    }
}

TrayIcon::~TrayIcon() {
    delete menu;
    m_instance = nullptr;
}
