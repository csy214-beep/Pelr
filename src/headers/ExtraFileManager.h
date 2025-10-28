/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once


#include <QObject>
#include <QString>
#include <QDir>
#include <QMap>
#include <QVector>
#include "CubismFramework.hpp"

class ExtraFileManager : public QObject {
    Q_OBJECT

public:
    explicit ExtraFileManager(QObject *parent = nullptr);

    ~ExtraFileManager();

    // 扫描并加载额外文件
    void loadExtraFiles(const QString &modelDir);

    // 获取可用的额外表情列表
    QVector<QString> getAvailableExpressions() const;

    // 获取可用的额外动作列表
    QVector<QString> getAvailableMotions() const;

    // 获取表情文件数据
    QByteArray getExpressionData(const QString &expressionName) const;

    // 获取动作文件数据
    QByteArray getMotionData(const QString &motionName) const;

private:
    // 递归扫描目录
    void scanDirectory(const QDir &dir);

    // 加载表情文件
    void loadExpressionFile(const QString &filePath);

    // 加载动作文件
    void loadMotionFile(const QString &filePath);

private:
    QMap<QString, QByteArray> _extraExpressions; // 额外表情文件数据
    QMap<QString, QByteArray> _extraMotions; // 额外动作文件数据
    QString _modelHomeDir;
};

