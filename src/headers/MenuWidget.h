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
#include <QScreen>
#include <QMouseEvent>
#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QVBoxLayout>

class MenuWidget : public QWidget {
    Q_OBJECT

public:
    QVBoxLayout *mainLayout;

    MenuWidget(QWidget *parent = nullptr);

    void showNearMouse();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:


protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    void setupUI();

    void applyStyles();

    bool left_button_down = false;
    QPoint dragStartPos;
};


