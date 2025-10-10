/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#pragma once

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <windows.h>
#include <QFileInfo>
#include <QPair>
#include <QFuture>
#include <QtConcurrent>
#include "tray.h"


static QFuture<void> launchByPathAsync(const QString &path) {
    return QtConcurrent::run([path]() {
        // 打开链接
        if (path.startsWith("http://") || path.startsWith("https://")) {
            QUrl url(path);
            if (QDesktopServices::openUrl(url)) {
                qDebug() << "URL opened successfully:" << path;
            } else {
                qDebug() << "Failed to open URL:" << path;
                QMetaObject::invokeMethod(qApp, [path]() {
                    if (auto tray = TrayIcon::instance()) {
                        tray->showMessage("PLauncher", "Failed to open URL: " + path,
                                          QSystemTrayIcon::Warning, 5000);
                    }
                }, Qt::QueuedConnection);
            }
            return;
        }

        // 打开文件或文件夹
        QFileInfo fileInfo(path);
        QDir dir(path);

        if (!fileInfo.exists()) {
            qDebug() << "Path does not exist:" << path;
            QMetaObject::invokeMethod(qApp, [path]() {
                if (auto tray = TrayIcon::instance()) {
                    tray->showMessage("PLauncher", "Path does not exist: " + path,
                                      QSystemTrayIcon::Warning, 5000);
                }
            }, Qt::QueuedConnection);
            return;
        }

        // 检查是否为文件夹路径
        if (dir.exists()) {
            // 使用ShellExecute打开文件夹
            if (ShellExecuteW(NULL, L"explore", reinterpret_cast<LPCWSTR>(path.utf16()),
                              NULL, NULL, SW_SHOWNORMAL) > (HINSTANCE) 32) {
                qDebug() << "Folder opened successfully:" << path;
            } else {
                qDebug() << "Failed to open folder:" << path;
                QMetaObject::invokeMethod(qApp, [path]() {
                    if (auto tray = TrayIcon::instance()) {
                        tray->showMessage("PLauncher", "Failed to open folder: " + path,
                                          QSystemTrayIcon::Warning, 5000);
                    }
                }, Qt::QueuedConnection);
            }
        } else {
            // 打开其他文件
            if (ShellExecuteW(NULL, L"open", reinterpret_cast<LPCWSTR>(path.utf16()), NULL, NULL, SW_SHOWNORMAL) == 0) {
                qDebug() << "Failed to open file:" << path;
                QMetaObject::invokeMethod(qApp, [path]() {
                    if (auto tray = TrayIcon::instance()) {
                        tray->showMessage("PLauncher", "Failed to open file: " + path,
                                          QSystemTrayIcon::Warning, 5000);
                    }
                }, Qt::QueuedConnection);
            } else {
                qDebug() << "File opened successfully:" << path;
            }
        }
    });
}

// 同步版本
static void launchByPath(const QString &path) {
    launchByPathAsync(path);
}
