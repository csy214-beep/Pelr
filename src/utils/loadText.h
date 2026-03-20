#pragma once
/*
PROJECT_NAME Pelr
PRODUCT_NAME CLion
NAME loadText
AUTHOR Pfolg
TIME 2026/3/20 21:56
*/

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>
#include <QRandomGenerator>
#include <QFileInfo>
#include "launcher.hpp"

#define DEFAULT_TEXT_FILE "assets/text/text.json"
#define USER_TEXT_FILE "user/text.json"

// 辅助函数：检查文件是否存在
static bool checkFileExists(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "File not found:" << filePath;
        return false;
    }
    return true;
}

// 辅助函数：从指定文件中按 key 获取随机字符串
static QString getRandomTextFromFile(const QString &filePath, const QString &keyName) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath << "-" << file.errorString();
        return QString();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "JSON parse failed for file:" << filePath;
        return QString();
    }

    QJsonObject root = doc.object();
    if (!root.contains(keyName) || !root[keyName].isArray()) {
        qWarning() << "JSON format error or missing key '" << keyName << "' in file:" << filePath;
        return QString();
    }

    QJsonArray targetArray = root[keyName].toArray();
    if (targetArray.isEmpty()) {
        qWarning() << "Array for key '" << keyName << "' is empty in file:" << filePath;
        return QString();
    }

    // Qt5 随机数生成器
    const int index = QRandomGenerator::global()->bounded(targetArray.size());
    return targetArray[index].toString();
}

// 主要加载函数：优先使用 user 文件，其次默认文件，最后硬编码默认语句
static QString loadText(const QString &keyName) {
    const QString fallbackText = "Hello World!";

    // 1. 优先尝试 user 文件
    if (checkFileExists(USER_TEXT_FILE)) {
        QString text = getRandomTextFromFile(USER_TEXT_FILE, keyName);
        if (!text.isEmpty()) {
            return text;
        }
        qWarning() << "User file is invalid, fallback to default file.";
    }

    // 2. 尝试默认文件
    if (checkFileExists(DEFAULT_TEXT_FILE)) {
        QString text = getRandomTextFromFile(DEFAULT_TEXT_FILE, keyName);
        if (!text.isEmpty()) {
            return text;
        }
        qWarning() << "Default file is invalid or missing key.";
    }

    // 3. 都失败则返回硬编码默认语句
    qWarning() << "No valid text source for key '" << keyName << "', using fallback.";
    return fallbackText;
}

// 初始化用户文件：若文件已存在则直接打开，否则复制默认文件后打开
// 注意：openFileForEditing 是外部已实现的打开文件的函数，需要用户自行提供
static void initUserTextFile() {
    // 确保 user 目录存在
    QDir dir;
    if (!dir.exists("user")) {
        if (!dir.mkpath("user")) {
            qCritical() << "Failed to create user directory.";
            return;
        }
    }

    // 检查 user 文件是否存在
    bool fileExists = QFile::exists(USER_TEXT_FILE);

    if (!fileExists) {
        // 复制默认文件
        if (QFile::exists(DEFAULT_TEXT_FILE)) {
            if (!QFile::copy(DEFAULT_TEXT_FILE, USER_TEXT_FILE)) {
                qCritical() << "Failed to copy default file to user directory.";
                return;
            }
            qDebug() << "User text file created at" << USER_TEXT_FILE;
        } else {
            qCritical() << "Default text file not found, cannot initialize user file.";
            return;
        }
    } else {
        qDebug() << "User text file already exists, opening directly.";
    }

    // 调用外部打开文件的模块
    launchByPath(USER_TEXT_FILE);
    qDebug() << "Please open file for editing:" << USER_TEXT_FILE;
}
