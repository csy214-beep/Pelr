/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
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
#include "llamaclient.h"
#include <QJsonObject>
#include <QJsonParseError>
#define MENU_DATA_FILE "user/menuData.dat"
#define CONFIG_DATA_FILE "user/configData.dat"
#define TODO_DATA_FILE "user/todoData.dat"
#define TODO_NOTIFY_FILE "user/todoNotify.dat"
#define TTS_CONFIG_FILE "user/ttsConfig.json"
#define OPEN_WEATHER_FILE "user/openWeather.json"
#define LLAMA_DATA_FILE "user/llamaConfig.json"

#define VERSION "20260503.14-dev" // 开发日期(内容变更起始日).release数量/顺序号(第几个版本).修订号(bug/feat次数)

struct colorPair
{
    QString forground;
    QString background;

    friend QDataStream &operator<<(QDataStream &out, const colorPair &data)
    {
        out << data.forground << data.background;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, colorPair &data)
    {
        in >> data.forground >> data.background;
        return in;
    }
};

struct ConfigData
{
    // basic
    QString model_path;
    int model_size = 150;
    int FPS = 30;
    int volume = 50;
    colorPair color_bubble = {"#ffffffff", "#ff00ffff"}; // 白字+蓝底 this is normal
    colorPair color_tray = {"#002fff", "#ff0000"};       // 静默：蓝；活动：红
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
    bool isShowThinkingBubble = false;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const ConfigData &data)
    {
        out << data.model_path << data.model_size << data.FPS << data.volume;

        // 分别序列化 QPair 的 first 和 second
        out << data.color_bubble;
        out << data.color_tray << data.music_tray_symbol;
        out << data.RandomInterval.first << data.RandomInterval.second;

        out << data.isStartUp << data.isListening << data.isLookingMouse
            << data.isStartStar << data.isRandomSpeech << data.isSaying
            << data.isHourAlarm << data.isTop << data.isTrayHourAlarm
            << data.isSilentBoot << data.isRecordWindowLocation
            << data.isMusicIcon << data.isShowThinkingBubble;

        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ConfigData &data)
    {
        in >> data.model_path >> data.model_size >> data.FPS >> data.volume;

        // 分别反序列化 QPair 的 first 和 second
        in >> data.color_bubble;
        in >> data.color_tray >> data.music_tray_symbol;
        in >> data.RandomInterval.first >> data.RandomInterval.second;

        in >> data.isStartUp >> data.isListening >> data.isLookingMouse >> data.isStartStar >> data.isRandomSpeech >>
            data.isSaying >> data.isHourAlarm >> data.isTop >> data.isTrayHourAlarm >> data.isSilentBoot >> data
                                                                                                                .isRecordWindowLocation >>
            data.isMusicIcon >> data.isShowThinkingBubble;

        return in;
    }
};

struct constConfigData
{
    const QString openai_edge_tts_github = "https://github.com/travisvn/openai-edge-tts";
    const QString openai_edge_tts_Voice_Samples = "https://tts.travisvn.com/";
    const QString iFlytek_tts_url = "https://console.xfyun.cn/services/tts";
    const QString tts_server = "tts_server.exe"; // local path
    const QString voicevox_help = "https://github.com/csy214-beep/Pelr/tree/master/docs/app-voicevox.md";
    const QString openWeather_url = "https://home.openweathermap.org/api_keys";
    const QString support_languages = "https://github.com/csy214-beep/Pelr/tree/master/docs/assets/languages.txt";
    const QString libretranslate_guide = "https://github.com/csy214-beep/Pelr/tree/master/docs/app-LibreTranslate.md";
    const QString tx_tr_guide = "https://github.com/csy214-beep/Pelr/tree/master/docs/app-txTr.md";
    // about
    const QString version = VERSION;
    const QString Gitee_repo_owner = "Pfolg";
    const QString Gitee_repo_name = "Pelr";
    const QString Github_repo_owner = "csy214-beep";
    const QString Github_repo_name = "Pelr";
    const QString team_link = "https://github.com/csy214-beep/Pelr/graphs/contributors";
    const QString website_link = "https://github.com/csy214-beep/Pelr";
    const QString feedback_link = "https://github.com/csy214-beep/Pelr/issues";
    const QString textFile = "user\\text.json"; // local path
    const QString VoiceFolder = "voice_files";  // local path
};

struct LlamaData
{
    QString model;
    QString systemPrompt;
    QString baseUrl;
    QString apiKey;
    QString promptFilePath;
    int maxContextMessages;
};

struct TodoData
{
    int category;
    QString title;
    QString content;
    QString deadline;
    QString remarks;
    bool isNotify;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const TodoData &data)
    {
        out << data.category << data.title << data.content << data.deadline << data.remarks << data.isNotify;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, TodoData &data)
    {
        in >> data.category >> data.title >> data.content >> data.deadline >> data.remarks >> data.isNotify;
        return in;
    }
};

struct MenuData
{
    QString category;
    QString name;
    QString path;
    QString icon;
    QString description;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const MenuData &data)
    {
        out << data.category << data.name << data.path << data.icon << data.description;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, MenuData &data)
    {
        in >> data.category >> data.name >> data.path >> data.icon >> data.description;
        return in;
    }

    friend bool operator!=(const MenuData &m1, const MenuData &m2)
    {
        return m1.category != m2.category || m1.name != m2.name || m1.path != m2.path || m1.icon != m2.icon || m1.description != m2.description;
    }
};

struct ToDoSettingData
{
    bool is_show_todo = true;
    bool is_notify_tray = true;

    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const ToDoSettingData &data)
    {
        out << data.is_show_todo << data.is_notify_tray;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ToDoSettingData &data)
    {
        in >> data.is_show_todo >> data.is_notify_tray;
        return in;
    }
};

struct TTSConfig
{
    // TTS
    int provider = 0; // 0: openai_edge_tts; 1: iFlytek 2: voicevox
    // openai_edge_tts
    QString speaker_openai_edge_tts = "zh-CN-XiaoxiaoNeural";
    double speed_openai_edge_tts = 1.0;
    // iFlytek
    QString iFlytek_APPID;
    QString iFlytek_APISecret;
    QString iFlytek_APIKey;
    QString iFlytek_speaker = "x4_yezi";
    // voicevox
    QString voicevox_dict_dir;
    QString voicevox_model;
    int voicevox_style_id;
    double voicevox_speed = 1.0;
    // TTS server
    bool isRunTTSServerOnStartUp = false;
    // Translate
    int tr_point;                // 0 libretranslate; 1 translators;
    QString tr_provider;         // translators
    QString tr_lang_translators; // translators
    QString tr_lang_libretranslate;
    QString tr_libretranslate_port = "5000";
    // 腾讯云机器翻译配置
    QString tr_tx_secret_id;   // SecretId
    QString tr_tx_secret_key;  // SecretKey
    QString tr_tx_region;      // 地域，默认 ap-guangzhou
    int tr_tx_project_id = 0;  // 项目 ID，默认 0
    QString tr_tx_source_lang; // 源语言，默认 "auto"
    QString tr_tx_target_lang; // 目标语言（tr_point == 2 时使用）
};

static QVector<QPair<QString, int>> TTSProviderList = {
    {"OpenAI-Edge-TTS", 0},
    {"iFlytek", 1},
    {"voicevox", 2},
};

static QVector<QPair<QString, int>> Translators = {
    {"libretranslate", 0},
    {"translators", 1},
    {"Tencent", 2},
};

struct OpenWeatherData
{
    // openWeather
    QString city;
    QString api_key;
};

class DataManager
{
private:
    DataManager() = default;                              // 私有构造函数
    ~DataManager() = default;                             // 私有析构函数
    DataManager(const DataManager &) = delete;            // 删除拷贝构造函数
    DataManager &operator=(const DataManager &) = delete; // 删除赋值运算符

protected:
    QList<MenuData> cached_menu_data;
    ConfigData basic_data;
    ToDoSettingData todo_setting_data;
    QString fontPath = ":/public/font/MapleMono-NF-CN-Medium.ttf";
    TTSConfig tts_config;
    OpenWeatherData openWeather_data;
    LlamaData llama_data;

public:
    QList<TodoData> todo_data;
    constConfigData const_config_data;
    QFont _font = loadFont();
    const QString Project_Name = "Pelr";

    static DataManager &instance()
    {
        static DataManager instance;
        return instance;
    }

    OpenWeatherData getOpenWeatherData()
    {
        readOpenWeatherData();
        return openWeather_data;
    }
    LlamaData getLlamaData()
    {
        readLlamaData();
        return llama_data;
    }
    TTSConfig getTTSConfig()
    {
        readTTSConfig();
        return tts_config;
    }

    ToDoSettingData getTodoSetting()
    {
        readTodoNotify();
        return todo_setting_data;
    }

    QList<MenuData> getMenuData()
    {
        readMenuData();
        return cached_menu_data;
    }

    ConfigData getBasicData()
    {
        readBasicData();
        return basic_data;
    }

    QList<TodoData> getTodoData()
    {
        readTodoData();
        return todo_data;
    }

    static void writeOpenWeatherData(const OpenWeatherData &opwdt);

    static void writeTTSConfig(const TTSConfig &ttsc);

    static void writeLlamaData(const LlamaData &llm);

    template <typename T>
    void writeData(const T &data)
    {
        QString filename;
        if constexpr (std::is_same_v<T, QList<MenuData>>)
        {
            filename = MENU_DATA_FILE;
        }
        else if constexpr (std::is_same_v<T, ConfigData>)
        {
            filename = CONFIG_DATA_FILE;
        }
        else if constexpr (std::is_same_v<T, QList<TodoData>>)
        {
            filename = TODO_DATA_FILE;
        }
        else
        {
            qCritical() << "Unsupported data type for writing:" << typeid(T).name();
            return; // 确保有返回
        }
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
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
        if constexpr (std::is_same_v<T, QList<MenuData>>)
        {
            cached_menu_data = data;
        }
        else if constexpr (std::is_same_v<T, ConfigData>)
        {
            basic_data = data;
        }
        else if constexpr (std::is_same_v<T, QList<TodoData>>)
        {
            todo_data = data;
        }
    }

    void writeData(ToDoSettingData setting);

protected:
    QJsonObject readJsonFile(const QString &filePath);

    void readLlamaData();

    void readOpenWeatherData();

    void readTTSConfig();

    void readTodoNotify();

    void readTodoData();

    static QFont loadFont();

    void readMenuData();

    void readBasicData();
};
