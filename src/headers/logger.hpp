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
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <QString>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>  // 线程安全锁，避免多线程写日志冲突

// 定义日志文件路径
#define QT_LOG_FILE "log/plauncher_qt.log"

// 定义日志等级枚举（和QtMsgType对应，便于使用）
enum class LogLevel {
    Debug = 0, // 调试信息（最低级）
    Info = 1, // 普通信息
    Warning = 2, // 警告
    Critical = 3, // 严重错误
    Fatal = 4 // 致命错误（最高级）
};

// 全局日志配置（线程安全）
static LogLevel g_logLevel = LogLevel::Debug; // 默认记录所有等级日志
static QMutex g_logMutex; // 日志写入互斥锁

// 输出到控制台的宏（如果启用）
#ifdef CONSOLE
#define LOG_TO_CONSOLE(txt) do { \
    QMutexLocker locker(&g_logMutex); \
    QTextStream out(stdout); \
    out << txt << Qt::endl; \
} while(0)
#else
#define LOG_TO_CONSOLE(txt)
#endif

// 初始化日志文件
static void initLogFile() {
    // 创建日志目录（包括父目录）
    QDir().mkpath("log");

    // 初始化时清空原有日志文件
    QFileInfo fileInfo(QT_LOG_FILE);
    if (fileInfo.exists()) {
        QFile::remove(QT_LOG_FILE);
    }
}

// 设置日志等级（外部调用接口，支持动态调整）
static void setLogLevel(LogLevel level) {
    QMutexLocker locker(&g_logMutex);
    g_logLevel = level;
}

// 获取当前日志等级（外部调用接口）
static LogLevel getLogLevel() {
    QMutexLocker locker(&g_logMutex);
    return g_logLevel;
}

// 将QtMsgType转换为自定义LogLevel
static LogLevel qtMsgTypeToLogLevel(QtMsgType type) {
    switch (type) {
        case QtDebugMsg: return LogLevel::Debug;
        case QtInfoMsg: return LogLevel::Info;
        case QtWarningMsg: return LogLevel::Warning;
        case QtCriticalMsg: return LogLevel::Critical;
        case QtFatalMsg: return LogLevel::Fatal;
        default: return LogLevel::Info;
    }
}

// 自定义消息处理函数（核心）
inline void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context)

    // 1. 转换日志类型，判断是否需要记录（低于设置等级则直接返回）
    LogLevel currentLevel = qtMsgTypeToLogLevel(type);
    if (currentLevel < g_logLevel) {
        return; // 不记录低于设置等级的日志
    }

    // 2. 加锁保证线程安全
    QMutexLocker locker(&g_logMutex);

    // 3. 拼接日志内容
    QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString logTypeStr;
    switch (type) {
        case QtDebugMsg:
            logTypeStr = "Debug";
            break;
        case QtInfoMsg:
            logTypeStr = "Info";
            break;
        case QtWarningMsg:
            logTypeStr = "Warning";
            break;
        case QtCriticalMsg:
            logTypeStr = "Critical";
            break;
        case QtFatalMsg:
            logTypeStr = "Fatal";
            break;
        default:
            logTypeStr = "Unknown";
            break;
    }
    QString txt = QString("[%1] [%2]: %3").arg(logTime).arg(logTypeStr).arg(msg);

    // 4. 输出到控制台（如果启用）
    LOG_TO_CONSOLE(txt);

    // 5. 写入日志文件
    QFile logFile(QT_LOG_FILE);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream textStream(&logFile);
        textStream.setCodec("UTF-8"); // 确保中文正常显示
        textStream << txt << Qt::endl;
        logFile.close();
    }
}
