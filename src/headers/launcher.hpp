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

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPair>
#include <QFuture>
#include <QtConcurrent>
#include "tray.h"


static QFuture<void> launchByPathAsync(const QString &path) {
    qDebug() << "launching: " << path;
    return QtConcurrent::run([path]() -> bool {
        try {
            bool success = false;

            // 对于URL，使用QDesktopServices
            if (path.startsWith("http://") || path.startsWith("https://") ||
                path.startsWith("ftp://") || path.startsWith("file://")) {
                success = QDesktopServices::openUrl(QUrl(path));
            }
                // 对于本地文件/文件夹，使用QDesktopServices
            else {
                QFileInfo fileInfo(path);
                if (fileInfo.exists()) {
                    QUrl localUrl = QUrl::fromLocalFile(QDir::toNativeSeparators(path));
                    success = QDesktopServices::openUrl(localUrl);
                } else {
                    qWarning() << "File or directory does not exist:" << path;
                    TrayIcon::instance()->showMessage(
                            QObject::tr("Warning"),
                            QObject::tr("File or directory does not exist: %1").arg(path),
                            QSystemTrayIcon::Warning, 5000);
                    return false;
                }
            }

            if (!success) {
                qWarning() << "Failed to open:" << path;
                TrayIcon::instance()->showMessage(
                        QObject::tr("Warning"),
                        QObject::tr("Failed to open: %1").arg(path),
                        QSystemTrayIcon::Warning, 5000);
                return false;
            }

            return true;
        } catch (const std::exception &e) {
            qCritical() << "Exception occurred while launching" << path << ":" << e.what();
            TrayIcon::instance()->showMessage(
                    QObject::tr("Error"),
                    QObject::tr("Exception occurred while launching: %1 \n%2").arg(path).arg(
                            e.what()),
                    QSystemTrayIcon::Critical, 5000);
            return false;
        } catch (...) {
            qCritical() << "Unknown exception occurred while launching" << path;
            TrayIcon::instance()->showMessage(
                    QObject::tr("Error"),
                    QObject::tr("Unknown exception occurred while launching: %1").arg(path),
                    QSystemTrayIcon::Critical, 5000);
            return false;
        }
    });
}

// 同步版本
static void launchByPath(const QString &path) {
    launchByPathAsync(path);
}
