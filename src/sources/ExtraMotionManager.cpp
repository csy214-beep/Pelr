/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "ExtraMotionManager.h"
#include <QDebug>

ExtraMotionManager *ExtraMotionManager::instance = nullptr;

ExtraMotionManager::ExtraMotionManager()
    : CustomMenu(nullptr), model(nullptr) {
    setTitle("额外内容");

    motionMenu = new QMenu("动作", this);
    expressionMenu = new QMenu("表情", this);

    addMenu(motionMenu);
    addMenu(expressionMenu);
}

ExtraMotionManager::~ExtraMotionManager() {
    instance = nullptr;
    delete instance;
}

ExtraMotionManager *ExtraMotionManager::getInstance() {
    if (!instance) {
        instance = new ExtraMotionManager();
    }
    return instance;
}

void ExtraMotionManager::destroyInstance() {
    delete instance;
}

void ExtraMotionManager::setModel(LAppModel *newModel) {
    model = newModel;
    refreshMenu();
}

void ExtraMotionManager::refreshMenu() {
    // 清空现有菜单
    motionMenu->clear();
    expressionMenu->clear();

    if (!model) {
        QAction *noModelAction = new QAction("没有加载模型", this);
        noModelAction->setEnabled(false);
        motionMenu->addAction(noModelAction);
        return;
    }

    // 添加动作
    int motionCount = 0;
    for (auto it = model->extraMotions.Begin(); it != model->extraMotions.End(); ++it) {
        if (it->Second == NULL) continue;

        QString motionName = QString::fromUtf8(it->First.GetRawString());
        QAction *action = new QAction(motionName, this);

        connect(action, &QAction::triggered, [this, motionName]() {
            if (model) {
                model->StartExtraMotion(motionName.toUtf8().constData(), PriorityNormal);
            }
        });

        motionMenu->addAction(action);
        motionCount++;
    }
    motionMenu->setTitle("动作 (" + QString::number(motionCount) + ")");

    // 添加表情
    int expressionCount = 0;
    for (auto it = model->extraExpressions.Begin(); it != model->extraExpressions.End(); ++it) {
        if (it->Second == NULL) continue;
        QString expressionName = QString::fromUtf8(it->First.GetRawString());
        QAction *action = new QAction(expressionName, this);

        connect(action, &QAction::triggered, [this, expressionName]() {
            if (model) {
                model->SetExtraExpression(expressionName.toUtf8().constData());
            }
        });

        expressionMenu->addAction(action);
        expressionCount++;
    }
    expressionMenu->setTitle("表情 (" + QString::number(expressionCount) + ")");
}
