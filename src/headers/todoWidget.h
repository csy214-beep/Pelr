/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QDateEdit>
#include <QModelIndex>
#include <QObject>
#include <QTableView>


namespace Ui {
    class todoWidget;
}

class todoWidget : public QWidget {
Q_OBJECT

public:
    QStandardItemModel *model_todo;
    QStandardItemModel *model_done;

    explicit todoWidget(QWidget *parent = nullptr);

    ~todoWidget();

    void initWidget();

    void loadAllData();

private slots:

    void addTodoItem(QStandardItemModel *model);

    void deleteSelectedItem(QTableView *view);

    void onTableViewDoubleClicked(const QModelIndex &index);

    void clearModelData(QStandardItemModel *model, bool cfm = true);

    void moveItem(QTableView *view);

    void saveAllData();

    void onCheckBoxClicked();

private:
    Ui::todoWidget *ui;
};
