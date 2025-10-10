/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#ifndef SETTING_H
#define SETTING_H

#include <QLineEdit>

#include "data.hpp"
#include <QWidget>
#include "DoubleClickableLabel.h"
#include  "VersionChecker.h"

namespace Ui {
    class setting;
}

class SettingWidget : public QWidget {
    Q_OBJECT

public:
    Ui::setting *ui;

    explicit SettingWidget(QWidget *parent = nullptr);

    ~SettingWidget();

    ConfigData getAllValues();

    void setAllValues(const ConfigData &data);

    void connectSignals();

    bool checkStartupLink();

private slots:
    void saveData();

    void selectModelPath();

    void selectColor(QLabel *label, QLineEdit *line, bool isBackground = true);

    void resetSetting();

    void onCheckBox1Clicked(bool flag = false);

    void onVersionCheckCompleted(bool isMatch, const QString &message);

    void onVersionCheckError(const QString &errorMessage);

private:
    VersionChecker *m_versionChecker;
};


#endif
