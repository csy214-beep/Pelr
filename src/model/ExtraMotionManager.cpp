
#include "ExtraMotionManager.h"
#include <QDebug>

ExtraMotionManager *ExtraMotionManager::instance = nullptr;

ExtraMotionManager::ExtraMotionManager()
    : CustomMenu(nullptr), model(nullptr)
{
    setTitle(tr("额外内容"));

    motionMenu = new QMenu(tr("动作"), this);
    expressionMenu = new QMenu(tr("表情"), this);

    addMenu(motionMenu);
    addMenu(expressionMenu);
}

ExtraMotionManager::~ExtraMotionManager()
{
    instance = nullptr;
    delete instance;
}

ExtraMotionManager *ExtraMotionManager::getInstance()
{
    if (!instance)
    {
        instance = new ExtraMotionManager();
    }
    return instance;
}

void ExtraMotionManager::destroyInstance()
{
    delete instance;
}

void ExtraMotionManager::setModel(LAppModel *newModel)
{
    model = newModel;
    refreshMenu();
}

void ExtraMotionManager::refreshMenu()
{
    // 清空现有菜单
    motionMenu->clear();
    expressionMenu->clear();

    if (!model)
    {
        QAction *noModelAction = new QAction(tr("没有加载模型"), this);
        noModelAction->setEnabled(false);
        motionMenu->addAction(noModelAction);
        return;
    }

    // 添加动作
    int motionCount = 0;
    for (auto it = model->extraMotions.Begin(); it != model->extraMotions.End(); ++it)
    {
        if (it->Second == NULL)
            continue;

        QString motionName = QString::fromUtf8(it->First.GetRawString());
        QAction *action = new QAction(motionName, this);

        connect(action, &QAction::triggered, [this, motionName]()
                {
            if (model) {
                model->StartExtraMotion(motionName.toUtf8().constData(), PriorityNormal);
            } });

        motionMenu->addAction(action);
        motionCount++;
    }
    motionMenu->setTitle(tr("动作 (%1)").arg(motionCount));

    // 添加表情
    int expressionCount = 0;
    for (auto it = model->extraExpressions.Begin(); it != model->extraExpressions.End(); ++it)
    {
        if (it->Second == NULL)
            continue;
        QString expressionName = QString::fromUtf8(it->First.GetRawString());
        QAction *action = new QAction(expressionName, this);

        connect(action, &QAction::triggered, [this, expressionName]()
                {
            if (model) {
                model->SetExtraExpression(expressionName.toUtf8().constData());
            } });

        expressionMenu->addAction(action);
        expressionCount++;
    }
    expressionMenu->setTitle(tr("表情 (%1)").arg(expressionCount));
}
