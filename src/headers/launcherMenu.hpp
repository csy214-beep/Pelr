/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QMenu>
#include "launcher.hpp"
#include "data.hpp"
#include <QAction>
#include <QIcon>
#include <QDebug>
#include <QPushButton>
#include "custommenu.h"

class launcherMenu : public CustomMenu {
Q_OBJECT

public:
    // 删除拷贝构造函数和赋值运算符
    launcherMenu(const launcherMenu &) = delete;

    launcherMenu &operator=(const launcherMenu &) = delete;

    // 获取单例实例的静态方法
    static launcherMenu *instance(QWidget *parent = nullptr) {
        static launcherMenu instance(parent);
        return &instance;
    }

    // 初始化菜单
    void initMenu() {
        clear();
        int star_count = 0, app_count = 0, link_count = 0, scripts_count = 0;
        menu_data = DataManager::instance().getMenuData();
        QString p1, p2, p3, p4;
        p1 = ":/assets/IconaMoon/General/Star - 24x24.png";
        p2 = ":/assets/IconaMoon/General/Apps - 24x24.png";
        p3 = ":/assets/IconaMoon/Interface/Link External - 24x24.png";
        p4 = ":/assets/IconaMoon/Files/File - 24x24.png";
        menu_Star = new CustomMenu("Star", this);
        menu_App = new CustomMenu("App", this);
        menu_Link = new CustomMenu("Link", this);
        menu_Scripts = new CustomMenu("Scripts", this);
        for (MenuData &item: menu_data) {
            if (item.name.isEmpty() || item.category.isEmpty() || item.path.isEmpty())continue;
            QAction *action = new QAction(QString(item.name), this);

            if (!item.description.isEmpty()) {
                action->setToolTip(item.description);
            }
            //连接信号槽
            connect(action, &QAction::triggered, [&]() {
                launchByPath(item.path);
            });
            if (item.category == menu_Star->title()) {
                menu_Star->addAction(action);
                action->setIcon(QIcon(p1));
                star_count++;
            } else if (item.category == menu_App->title()) {
                menu_App->addAction(action);
                action->setIcon(QIcon(p2));
                app_count++;
            } else if (item.category == menu_Link->title()) {
                menu_Link->addAction(action);
                action->setIcon(QIcon(p3));
                link_count++;
            } else if (item.category == menu_Scripts->title()) {
                menu_Scripts->addAction(action);
                action->setIcon(QIcon(p4));
                scripts_count++;
            }
            if (!item.icon.isEmpty()) {
                action->setIcon(QIcon(item.icon));
            }
        }

        menu_Star->setTitle("Star (" + QString::number(star_count) + ")");
        menu_Star->setIcon(QIcon(p1));
        menu_App->setTitle("App (" + QString::number(app_count) + ")");
        menu_App->setIcon(QIcon(p2));
        menu_Link->setTitle("Link (" + QString::number(link_count) + ")");
        menu_Link->setIcon(QIcon(p3));
        menu_Scripts->setTitle("Scripts (" + QString::number(scripts_count) + ")");
        menu_Scripts->setIcon(QIcon(p4));
        addMenu(menu_Star);
        addMenu(menu_App);
        addMenu(menu_Link);
        addMenu(menu_Scripts);
        qDebug() << "menu loaded";
    }

    // 刷新菜单数据
    void refreshMenu() {
        qDebug() << "refresh menu";
        initMenu();
    }

protected:
    // 将构造函数设为protected，确保只能通过instance方法创建
    explicit launcherMenu(QWidget *parent = nullptr) : CustomMenu(parent) {
        initMenu();
        applyStyle();
    }

    QMenu *menu_Star, *menu_App, *menu_Link, *menu_Scripts;
    QList<MenuData> menu_data;
};
