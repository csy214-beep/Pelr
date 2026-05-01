/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
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
#include "VersionChecker.h"
#include <QtWidgets/QSlider>
#include "pyLang.h"
#include "ui_setting.h"

namespace Ui
{
    class setting;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);

    ~SettingWidget();

    ConfigData getAllValues();

    void setAllValues(const ConfigData &data);

    void setTTSConfig(const TTSConfig &data) const;
    void setLlamaData(const LlamaData &llm) const;

    void setOpenWeatherData(const OpenWeatherData &data) const;

    TTSConfig getTTSConfigValue() const;
    LlamaData getLlamaDataValue() const;

    OpenWeatherData getOpenWeatherDataValue();

    void connectSignals();

    bool checkStartupLink();

    // horizontalSlider
    QSlider *getHorizontalSlider();

private slots:
    void saveData();

    void selectModelPath();

    QString selectColor();

    void resetSetting();

    void onCheckBox1Clicked(bool flag = false);

    void onLogLevelChanged();

private:
    PyLang *m_langClient;
    VersionChecker *m_versionChecker;
    Ui::setting *ui;
};
