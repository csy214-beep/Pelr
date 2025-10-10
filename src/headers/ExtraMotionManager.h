#pragma once
#include <QMenu>
#include "LAppModel.hpp"

class ExtraMotionManager : public QMenu {
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
