/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
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
#include  <QFontDatabase>

#define MENU_DATA_FILE "user/menuData.dat"
#define CONFIG_DATA_FILE "user/configData.dat"
#define TODO_DATA_FILE "user/todoData.dat"
#define TODO_NOTIFY_FILE "user/todoNotify.dat"

struct ConfigData {
    //basic
    QString model_path;
    int model_size = 150;
    int FPS = 30;
    int volume = 50;
    QPair<QString, QString> color_bubble;
    QPair<QString, QString> color_keyLabel;
    //bool
    bool isStartUp = false;
    bool isListening = false;
    bool isLookingMouse = true;
    bool isStartStar = false;
    bool isRandomSpeech = true;
    bool isSaying = true;
    bool isHourAlarm = true;
    bool isTop = false;
    bool isTrayHourAlarm = false;
    //TTS
    QString APPID;
    QString APISecret;
    QString APIKey;
    QString speaker = "x4_yezi";
    //ollama
    QString model;
    QString role;
    QString customRoleDesc;
    // openWeather
    QString city;
    QString api_key;


    // 重载运算符以便使用QDataStream进行序列化
    friend QDataStream &operator<<(QDataStream &out, const ConfigData &data) {
        out << data.model_path << data.model_size << data.FPS << data.volume;

        // 分别序列化 QPair 的 first 和 second
        out << data.color_bubble.first << data.color_bubble.second;
        out << data.color_keyLabel.first << data.color_keyLabel.second;

        out << data.isStartUp << data.isListening << data.isLookingMouse
            << data.isStartStar << data.isRandomSpeech << data.isSaying
            << data.isHourAlarm << data.isTop << data.isTrayHourAlarm;

        out << data.APPID << data.APISecret << data.APIKey << data.speaker;
        out << data.model << data.role << data.customRoleDesc;
        out << data.city << data.api_key;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ConfigData &data) {
        in >> data.model_path >> data.model_size >> data.FPS >> data.volume;

        // 分别反序列化 QPair 的 first 和 second
        in >> data.color_bubble.first >> data.color_bubble.second;
        in >> data.color_keyLabel.first >> data.color_keyLabel.second;

        in >> data.isStartUp >> data.isListening >> data.isLookingMouse
           >> data.isStartStar >> data.isRandomSpeech >> data.isSaying
           >> data.isHourAlarm >> data.isTop >> data.isTrayHourAlarm;

        in >> data.APPID >> data.APISecret >> data.APIKey >> data.speaker;
        in >> data.model >> data.role >> data.customRoleDesc;
        in >> data.city >> data.api_key;
        return in;
    }
};

struct constConfigData {
    const QString tts_url = "https://console.xfyun.cn/services/tts";
    const QString tts_server = "tts_server.exe"; //local path
    const QString ollama_url = "https://ollama.com/";
    const QString openWeather_url = "https://home.openweathermap.org/api_keys";
    //about
    const QString version = "ver20251114-8";//开发日期(内容变更起始日)_release数量/顺序号(第几个版本)
    const QString name = "PLauncher";
    const QString repo_owner = "Pfolg";
    const QString team_link = "https://gitee.com/Pfolg/plauncher/contributors?ref=master";
    const QString website_link = "https://gitee.com/Pfolg/plauncher";
    const QString feedback_link = "https://gitee.com/Pfolg/plauncher/issues";
    const QString textFile = "assets\\text"; //local path
    const QString VoiceFolder = "voice_files"; //local path
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

class DataManager {
private:
    DataManager() = default; // 私有构造函数
    ~DataManager() = default; // 私有析构函数
    DataManager(const DataManager &) = delete; // 删除拷贝构造函数
    DataManager &operator=(const DataManager &) = delete; // 删除赋值运算符
protected:
    QList<MenuData> cached_menu_data;
    ConfigData basic_data;
    QString fontPath = ":/assets/font/MapleMono-NF-CN-Medium.ttf";

public:
    QList<TodoData> todo_data;
    bool is_todo_notify = false;
    constConfigData const_config_data;
    QFont _font = loadFont();

    static DataManager &instance() {
        static DataManager instance;
        return instance;
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
        //同时读取
        readTodoNotify();
        readTodoData();
        return todo_data;
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
            QMessageBox::critical(nullptr, "Error", "写入数据失败！");
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15); // 设置流版本以确保兼容性

        // 写入数据
        out << data;
        // 更新缓存
        if constexpr (std::is_same_v<T, QList<MenuData> >) {
            cached_menu_data = data;
        } else if constexpr (std::is_same_v<T, ConfigData>) {
            basic_data = data;
        } else if constexpr (std::is_same_v<T, QList<TodoData> >) {
            todo_data = data;
        }
    }

    void writeData(bool is_notify) {
        QFile file(TODO_NOTIFY_FILE);
        if (!file.open(QIODevice::WriteOnly)) {
            // 无法打开文件进行写入
            QMessageBox::critical(nullptr, "Error", "写入数据失败！");
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15); // 设置流版本以确保兼容性

        // 写入数据
        out << is_notify;
        // 更新缓存
        is_todo_notify = is_notify;
    }

protected:
    void readTodoNotify() {
        QFile file(TODO_NOTIFY_FILE);
        if (!file.open(QIODevice::ReadOnly)) {
            // qDebug() <<__func__<< "读取菜单数据失败";
            return; // 文件不存在或无法打开，返回空列表
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
        in >> is_todo_notify;
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
                    qInfo() << "成功加载字体:" << boldFamilies.at(0);
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
            qDebug() << "读取菜单数据失败";
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
