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
#include "LAppModel.hpp"
#include "custommenu.h"

class ExtraMotionManager : public CustomMenu {
    Q_OBJECT

public:
    static ExtraMotionManager *getInstance();

    static void destroyInstance();

    void setModel(LAppModel *newModel);

private:
    ExtraMotionManager();

    ~ExtraMotionManager();

    void refreshMenu();

    static ExtraMotionManager *instance;
    LAppModel *model;
    QMenu *motionMenu;
    QMenu *expressionMenu;
};
