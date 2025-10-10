/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#pragma once
#include <QScreen>
#include <QLabel>
#include <QCloseEvent>
#include <QString>
#include <QTimer>

class KeyLabelWidget : public QWidget {
    Q_OBJECT

public:
    KeyLabelWidget(QWidget *parent = nullptr);

    ~KeyLabelWidget();

    QLabel *keyLabel;
    QLabel *mouseLabel;
    QTimer *fadeTimer1;
    QTimer *fadeTimer2;

    void on_keyRelease(QString keyName, QString modifiersName);

    void on_mouseMove(int x, int y);

public slots:
    void updateWindowLocation(int f_x, int f_y, int f_w, int f_h);
};

