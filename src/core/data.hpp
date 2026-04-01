/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 *
 * CORE:
 * this file is used to manage data, including menu data and basic data
 * this file is very important, no data no program!
 */
#pragma once

#include <qdatastream.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QFontDatabase>
#include "ollamaclient.h"
#include <QJsonObject>
#include <QJsonParseError>
#define MENU_DATA_FILE "user/menuData.dat"
#define CONFIG_DATA_FILE "user/configData.dat"
#define TODO_DATA_FILE "user/todoData.dat"
#define TODO_NOTIFY_FILE "user/todoNotify.dat"
#define TTS_CONFIG_FILE "user/ttsConfig.json"
#define OPEN_WEATHER_FILE "user/openWeather.json"

#define VERSION "20260219.11.17" // 开发日期(内容变更起始日).release数量/顺序号(第几个版本).修订号(bug/feat次数)
// todo: 多语言支持 非紧急

struct colorPair {
    QString forground;
    QString background;

    friend QDataStream &operator<<(QDataStream &out, const colorPair &data) {
        out << data.forground << data.background;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, colorPair &data) {
        in >> data.forground >> data.background;
        return in;
    }
};

struct ConfigData {
    // basic
    QString model_path;
    int model_size = 150;
    int FPS = 30;
    int volume = 50;
    colorPair color_bubble = {"#FFFFFFCC", "#0c9beef3"}; // 白+蓝
    colorPair color_tray = {"#002fff", "#ff0000"}; // 静默：蓝；活动：红
    QPair<int, int> RandomInterval = {10, 25};
    QString music_tray_symbol = "♫";
    // bool
    bool isStartUp = false;
    bool isListening = false;
    bool isLookingMouse = true;
    bool isStartStar = false;
    bool isRandomSpeech = true;
    bool isSaying = true;
    bool isHourAlarm = true;
    bool isTop = false;
    bool isTrayHourAlarm = false;
    bool isSilentBoot = false;
    bool isRecordWindowLocation = false;
    bool isMusicIcon = false;

    // ollama
    QString model;
    OllamaClient::Role role = OllamaClient::DefaultCoder;
    QString customRoleDesc;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const ConfigData &data) {
        out << data.model_path << data.model_size << data.FPS << data.volume;

        // 分别序列化 QPair 的 first 和 second
        out << data.color_bubble;
        out << data.color_tray << data.music_tray_symbol;
        out << data.RandomInterval.first << data.RandomInterval.second;

        out << data.isStartUp << data.isListening << data.isLookingMouse
                << data.isStartStar << data.isRandomSpeech << data.isSaying
                << data.isHourAlarm << data.isTop << data.isTrayHourAlarm
                << data.isSilentBoot << data.isRecordWindowLocation << data.isMusicIcon;

        out << data.model << data.role << data.customRoleDesc;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ConfigData &data) {
        in >> data.model_path >> data.model_size >> data.FPS >> data.volume;

        // 分别反序列化 QPair 的 first 和 second
        in >> data.color_bubble;
        in >> data.color_tray >> data.music_tray_symbol;
        in >> data.RandomInterval.first >> data.RandomInterval.second;

        in >> data.isStartUp >> data.isListening >> data.isLookingMouse >> data.isStartStar >> data.isRandomSpeech >>
                data.isSaying >> data.isHourAlarm >> data.isTop >> data.isTrayHourAlarm >> data.isSilentBoot >> data
                .isRecordWindowLocation >>
                data.isMusicIcon;

        in >> data.model >> data.role >> data.customRoleDesc;
        return in;
    }
};

struct constConfigData {
    const QString openai_edge_tts_github = "https://github.com/travisvn/openai-edge-tts";
    const QString openai_edge_tts_Voice_Samples = "https://tts.travisvn.com/";
    const QString iFlytek_tts_url = "https://console.xfyun.cn/services/tts";
    const QString tts_server = "tts_server.exe"; // local path
    const QString ollama_url = "https://ollama.com/";
    const QString openWeather_url = "https://home.openweathermap.org/api_keys";
    // about
    const QString version = VERSION;
    const QString name = "Pelr";
    const QString repo_owner = "Pfolg";
    const QString team_link = "https://gitee.com/Pfolg/Pelr/contributors?ref=master";
    const QString website_link = "https://gitee.com/Pfolg/Pelr";
    const QString feedback_link = "https://gitee.com/Pfolg/Pelr/issues";
    const QString textFile = "user\\text.json"; // local path
    const QString VoiceFolder = "voice_files"; // local path
};

struct TodoData {
    int category;
    QString title;
    QString content;
    QString deadline;
    QString remarks;
    bool isNotify;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const TodoData &data) {
        out << data.category << data.title << data.content << data.deadline << data.remarks << data.isNotify;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, TodoData &data) {
        in >> data.category >> data.title >> data.content >> data.deadline >> data.remarks >> data.isNotify;
        return in;
    }
};

struct MenuData {
    QString category;
    QString name;
    QString path;
    QString icon;
    QString description;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const MenuData &data) {
        out << data.category << data.name << data.path << data.icon << data.description;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, MenuData &data) {
        in >> data.category >> data.name >> data.path >> data.icon >> data.description;
        return in;
    }

    friend bool operator!=(const MenuData &m1, const MenuData &m2) {
        return m1.category != m2.category || m1.name != m2.name || m1.path != m2.path || m1.icon != m2.icon || m1.
               description != m2.description;
    }
};

struct ToDoSettingData {
    bool is_show_todo = true;
    bool is_notify_tray = true;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const ToDoSettingData &data) {
        out << data.is_show_todo << data.is_notify_tray;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ToDoSettingData &data) {
        in >> data.is_show_todo >> data.is_notify_tray;
        return in;
    }
};

struct TTSConfig {
    // TTS
    int provider = 0; // 0: openai_edge_tts; 1: iFlytek
    // openai_edge_tts
    QString speaker_openai_edge_tts = "zh-CN-XiaoxiaoNeural";
    double speed_openai_edge_tts = 1.0;
    // iFlytek
    QString iFlytek_APPID;
    QString iFlytek_APISecret;
    QString iFlytek_APIKey;
    QString iFlytek_speaker = "x4_yezi";
    // TTS server
    bool isRunTTSServerOnStartUp = false;
};

static QVector<QPair<QString, int> > TTSProviderList = {
    {"OpenAI-Edge-TTS", 0},
    {"iFlytek", 1}
};

struct OpenWeatherData {
    // openWeather
    QString city;
    QString api_key;
};

class DataManager {
private:
    DataManager() = default; // 私有构造函数
    ~DataManager() = default; // 私有析构函数
    DataManager(const DataManager &) = delete; // 删除拷贝构造函数
    DataManager &operator=(const DataManager &) = delete; // 删除赋值运算符

protected:
    QList<MenuData> cached_menu_data;
    ConfigData basic_data;
    ToDoSettingData todo_setting_data;
    QString fontPath = ":/assets/font/MapleMono-NF-CN-Medium.ttf";
    TTSConfig tts_config;
    OpenWeatherData openWeather_data;

public:
    QList<TodoData> todo_data;
    constConfigData const_config_data;
    QFont _font = loadFont();
    const QString Project_Name = "Pelr";

    static DataManager &instance() {
        static DataManager instance;
        return instance;
    }

    OpenWeatherData getOpenWeatherData() {
        readOpenWeatherData();
        return openWeather_data;
    }

    TTSConfig getTTSConfig() {
        readTTSConfig();
        return tts_config;
    }

    ToDoSettingData getTodoSetting() {
        readTodoNotify();
        return todo_setting_data;
    }

    QList<MenuData> getMenuData() {
        readMenuData();
        return cached_menu_data;
    }

    ConfigData getBasicData() {
        readBasicData();
        return basic_data;
    }

    QList<TodoData> getTodoData() {
        readTodoData();
        return todo_data;
    }

    static void writeOpenWeatherData(const OpenWeatherData &opwdt) {
        QJsonObject json_object;
        json_object.insert("city", opwdt.city);
        json_object.insert("api_key", opwdt.api_key);
        QFile file(OPEN_WEATHER_FILE);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败：%1").arg(OPEN_WEATHER_FILE));
            qCritical() << "write OpenWeather Config failed: can not open file";
            return; // 无法打开文件进行写入
        }
        QJsonDocument json_doc(json_object);
        file.write(json_doc.toJson());
        file.close();
    }

    static void writeTTSConfig(const TTSConfig &ttsc) {
        QJsonObject json_object;
        json_object.insert("provider", ttsc.provider);
        json_object.insert("speaker_openai_edge_tts", ttsc.speaker_openai_edge_tts);
        json_object.insert("speed_openai_edge_tts", ttsc.speed_openai_edge_tts);
        json_object.insert("iFlytek_APPID", ttsc.iFlytek_APPID);
        json_object.insert("iFlytek_APISecret", ttsc.iFlytek_APISecret);
        json_object.insert("iFlytek_APIKey", ttsc.iFlytek_APIKey);
        json_object.insert("iFlytek_speaker", ttsc.iFlytek_speaker);
        json_object.insert("isRunTTSServerOnStartUp", ttsc.isRunTTSServerOnStartUp);
        QFile file(TTS_CONFIG_FILE);
        if (!file.open(QIODevice::WriteOnly)) {
            // 无法打开文件进行写入
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败: %1").arg(TTS_CONFIG_FILE));
            qCritical() << "write tts config failed: can not open file";
            return;
        }
        QJsonDocument json_doc(json_object);
        file.write(json_doc.toJson());
        file.close();
    }

    template<typename T>
    void writeData(const T &data) {
        QString filename;
        if constexpr (std::is_same_v<T, QList<MenuData> >) {
            filename = MENU_DATA_FILE;
        } else if constexpr (std::is_same_v<T, ConfigData>) {
            filename = CONFIG_DATA_FILE;
        } else if constexpr (std::is_same_v<T, QList<TodoData> >) {
            filename = TODO_DATA_FILE;
        } else {
            qCritical() << "Unsupported data type for writing:" << typeid(T).name();
            return; // 确保有返回
        }
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            // 无法打开文件进行写入
            QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败: %1").arg(filename));
            qCritical() << "write data failed: can not open file";
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15); // 设置流版本以确保兼容性

        // 写入数据
        out << data;
        file.close();
        // 更新缓存
        if constexpr (std::is_same_v<T, QList<MenuData> >) {
            cached_menu_data = data;
        } else if constexpr (std::is_same_v<T, ConfigData>) {
            basic_data = data;
        } else if constexpr (std::is_same_v<T, QList<TodoData> >) {
            todo_data = data;
        }
    }

    void writeData(ToDoSettingData setting) {
        QFile file(TODO_NOTIFY_FILE);
        if (!file.open(QIODevice::WriteOnly)) {
            // 无法打开文件进行写入
            QMessageBox::critical(nullptr, "Error", "写入数据失败！");
            qCritical() << "write todo setting data failed: can not open file";
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15); // 设置流版本以确保兼容性

        // 写入数据
        out << setting;
        file.close();
        // 更新缓存
        todo_setting_data = setting;
    }

protected:
    void readOpenWeatherData() {
        QFile file(OPEN_WEATHER_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file:" << OPEN_WEATHER_FILE << "error:" << file.errorString();
            return; // 文件不存在或无法打开
        }
        QByteArray fileData = file.readAll();
        file.close();

        if (fileData.isEmpty()) {
            qWarning() << "OpenWeather Config file is empty:" << OPEN_WEATHER_FILE;
            return; // 空文件
        }

        QJsonParseError parseError;
        QJsonDocument json_doc = QJsonDocument::fromJson(fileData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qCritical() << "JSON Parse Error:" << parseError.errorString()
                    << "at offset:" << parseError.offset;
            return; // JSON格式错误
        }

        if (!json_doc.isObject()) {
            qWarning() << "OpenWeather Config file root element is not an object";
            return; // 根元素必须是对象
        }

        QJsonObject json_object = json_doc.object();

        // 逐个读取并验证必要字段
        openWeather_data.api_key = json_object.value("api_key").toString("");
        if (openWeather_data.api_key.isEmpty()) {
            qWarning() << "api_key in Config file is empty or not exist";
        }
        openWeather_data.city = json_object.value("city").toString("");
    }

    void readTTSConfig() {
        QFile file(TTS_CONFIG_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file:" << TTS_CONFIG_FILE << "错误:" << file.errorString();
            return; // 文件不存在或无法打开
        }

        QByteArray fileData = file.readAll();
        file.close();

        if (fileData.isEmpty()) {
            qWarning() << "TTS Config file is empty:" << TTS_CONFIG_FILE;
            return; // 空文件
        }

        QJsonParseError parseError;
        QJsonDocument json_doc = QJsonDocument::fromJson(fileData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qCritical() << "JSON Parse Error:" << parseError.errorString()
                    << "at offset:" << parseError.offset;
            return; // JSON格式错误
        }

        if (!json_doc.isObject()) {
            qWarning() << "TTS Config file root element is not an object";
            return; // 根元素必须是对象
        }

        QJsonObject json_object = json_doc.object();

        // 逐个读取并验证必要字段
        tts_config.provider = json_object.value("provider").toInt(0);
        if (tts_config.provider < 0 || tts_config.provider > 1) {
            tts_config.provider = 0;
        }
        tts_config.speaker_openai_edge_tts = json_object.value("speaker_openai_edge_tts").toString(
            "zh-CN-XiaoxiaoNeural");
        tts_config.speed_openai_edge_tts = json_object.value("speed_openai_edge_tts").toDouble(1.0);
        tts_config.iFlytek_APPID = json_object.value("iFlytek_APPID").toString("");
        tts_config.iFlytek_APISecret = json_object.value("iFlytek_APISecret").toString("");
        tts_config.iFlytek_APIKey = json_object.value("iFlytek_APIKey").toString("");
        tts_config.iFlytek_speaker = json_object.value("iFlytek_speaker").toString("x4_yezi");
        if (tts_config.iFlytek_APPID.isEmpty() || tts_config.iFlytek_APISecret.isEmpty() || tts_config.iFlytek_APIKey.
            isEmpty())
            tts_config.provider = 0;
        tts_config.isRunTTSServerOnStartUp = json_object.value("isRunTTSServerOnStartUp").toBool(false);
    }

    void readTodoNotify() {
        QFile file(TODO_NOTIFY_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            // qDebug() <<__func__<< "读取菜单数据失败";
            return; // 文件不存在或无法打开，返回空列表
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
        in >> todo_setting_data;
        file.close();
    }

    void readTodoData() {
        QList<TodoData> _todo_data;
        QFile file(TODO_DATA_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            // qDebug() <<__func__<< "读取菜单数据失败";
            return; // 文件不存在或无法打开，返回空列表
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
        in >> _todo_data;
        file.close();
        todo_data = _todo_data;
    }

    static QFont loadFont() {
        QString boldFontPath = ":/assets/font/MapleMono-NF-CN-Medium.ttf";
        if (QFile::exists(boldFontPath)) {
            int boldFontId = QFontDatabase::addApplicationFont(boldFontPath);
            if (boldFontId != -1) {
                QStringList boldFamilies = QFontDatabase::applicationFontFamilies(boldFontId);
                if (!boldFamilies.isEmpty()) {
                    qInfo() << "load font success:" << boldFamilies.at(0);
                    return QFont(boldFamilies.at(0));
                }
            }
        }
        return QFont();
    }

    void readMenuData() {
        QList<MenuData> menu_data;
        QFile file(MENU_DATA_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "menu data file not exist or can not open, return empty list";
            return; // 文件不存在或无法打开，返回空列表
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
        in >> menu_data;
        file.close();
        cached_menu_data = menu_data;
    }

    void readBasicData() {
        ConfigData _basic_data;
        QFile file(CONFIG_DATA_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            // qDebug() <<__func__<< "读取菜单数据失败";
            return; // 文件不存在或无法打开，返回空列表
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
        in >> _basic_data;
        file.close();
        basic_data = _basic_data;
    }
};
