/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef MANAGESTART_H
#define MANAGESTART_H

#include  <QWidget>
#include <QStandardItemModel>
#include "editor.h"
#include "data.hpp"
#include <QList>
#include <QCloseEvent>

namespace Ui {
    class manageStart;
}

class ManageStartWidget : public QWidget {
    Q_OBJECT

public:
    bool isSaved;
    QStandardItemModel *dataModel;
    QStandardItem *p1_star, *p2_app, *p3_link, *p4_scripts;
    EditorWidget *editorWidget;

    void addItemToList(QStandardItem *parent, QList<QString> &item);

    explicit ManageStartWidget(QWidget *parent = nullptr);

    ~ManageStartWidget();

    void initWidget();

    void getAllItems(QList<MenuData> &data);

    void loadMenuData();

public slots:
    void saveMenuData();

protected slots:
    void editObj();

    void showEditor();


    void onEditorAccepted();

    void deleteSelectedItem();

    void onEditorAcceptedForEdit();

private:
    QString currentCategory; // 当前选中项的分类
    QString currentName; // 当前选中项的名称
    int currentRow; // 当前选中项的行号
    Ui::manageStart *ui;
};

#endif
