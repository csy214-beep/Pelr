/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef ADJUSTLABEL_HPP
#define ADJUSTLABEL_HPP
#include <QWidget>
#include <QVBoxLayout>
#include <QList>

template<typename T>
void adjustLabel(QWidget *parent, int up, int down, int left, int right, const QList<T *> &ws) {
    // 清除现有布局
    if (parent->layout()) {
        delete parent->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(parent);
    // 上方空白 (拉伸因子 = up)
    mainLayout->addStretch(up);

    // 水平布局容器
    QHBoxLayout *hContainer = new QHBoxLayout();
    // 左侧空白 (拉伸因子 = left)
    hContainer->addStretch(left);

    // 文字容器
    QVBoxLayout *textLayout = new QVBoxLayout();
    for (T *w: ws) {
        textLayout->addWidget(w);
    }
    hContainer->addLayout(textLayout);

    // 右侧空白 (拉伸因子 = right)
    hContainer->addStretch(right);

    // 添加水平容器到主布局
    mainLayout->addLayout(hContainer);

    // 下方空白 (拉伸因子 = down)
    mainLayout->addStretch(down);

    parent->setLayout(mainLayout);
}

template<typename T>
void adjustLabel(QWidget *parent, int up, int down, int left, int right, T * &w) {
    // 清除现有布局
    if (parent->layout()) {
        delete parent->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(parent);
    // 上方空白 (拉伸因子 = up)
    mainLayout->addStretch(up);

    // 水平布局容器
    QHBoxLayout *hContainer = new QHBoxLayout();
    // 左侧空白 (拉伸因子 = left)
    hContainer->addStretch(left);

    // 文字容器
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(w);
    hContainer->addLayout(textLayout);

    // 右侧空白 (拉伸因子 = right)
    hContainer->addStretch(right);

    // 添加水平容器到主布局
    mainLayout->addLayout(hContainer);

    // 下方空白 (拉伸因子 = down)
    mainLayout->addStretch(down);

    parent->setLayout(mainLayout);
}
#endif // ADJUSTLABEL_HPP
