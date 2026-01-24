/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QLineEdit>

#include "data.hpp"
#include <QWidget>
#include "DoubleClickableLabel.h"
#include  "VersionChecker.h"
#include <QtWidgets/QSlider>

#include "ui_setting.h"


namespace Ui {
    class setting;
}

class SettingWidget : public QWidget {
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);

    ~SettingWidget();

    ConfigData getAllValues();

    void setAllValues(const ConfigData &data);

    void setTTSConfig(const TTSConfig &data) const;

    void setOpenWeatherData(const OpenWeatherData &data) const;

    TTSConfig getTTSConfigValue() const;

    OpenWeatherData getOpenWeatherDataValue();

    void connectSignals();

    bool checkStartupLink();

    //horizontalSlider
    QSlider *getHorizontalSlider();

private slots:
    void saveData();

    void selectModelPath();

    void selectColor(QLabel *label, QLineEdit *line, bool isBackground = true);

    void resetSetting();

    void onCheckBox1Clicked(bool flag = false);

    void onVersionCheckCompleted(bool isMatch, const QString &message);

    void onVersionCheckError(const QString &errorMessage);

    void onLogLevelChanged();

private:
    VersionChecker *m_versionChecker;
    Ui::setting *ui;
};

