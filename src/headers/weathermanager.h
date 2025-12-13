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

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// 定义返回的天气数据结构体
struct WeatherData {
    QString city; // 城市名称
    double temperature; // 温度（摄氏度）
    QString description; // 天气描述
    double humidity; // 湿度（%）
    QString error; // 错误信息（若为空表示成功）

    WeatherData() : temperature(0), humidity(0) {
    }
};

class WeatherManager : public QObject {
    Q_OBJECT

public:
    // 获取单例实例（线程安全）
    static WeatherManager *instance();

    // 获取天气数据
    WeatherData getWeatherData(const QString &city, const QString &apiKey);

private:
    // 私有构造函数和析构函数
    WeatherManager(QObject *parent = nullptr);

    ~WeatherManager();

    // 禁止拷贝和赋值
    WeatherManager(const WeatherManager &) = delete;

    WeatherManager &operator=(const WeatherManager &) = delete;

    QNetworkAccessManager *manager; // 网络请求管理器
};
