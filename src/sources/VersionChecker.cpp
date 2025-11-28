/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "VersionChecker.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "data.hpp"

void checkSslSupport() {
    qDebug() << "SSL support:" << QSslSocket::supportsSsl();
    qDebug() << "SSL version:" << QSslSocket::sslLibraryVersionString();
    qDebug() << "SSL build version:" << QSslSocket::sslLibraryBuildVersionString();
}

VersionChecker::VersionChecker(QObject *parent)
        : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {
    checkSslSupport();
    // 连接网络请求完成信号
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &VersionChecker::onReplyFinished);
}

VersionChecker::~VersionChecker() {
}

void VersionChecker::checkVersionMatch(const QString &localVersion) {
    m_localVersion = localVersion;

    // 配置参数
    const QString REPO_OWNER = DataManager::instance().const_config_data.repo_owner;
    const QString REPO_NAME = DataManager::instance().const_config_data.name;


    // getLatestGithubVersion(REPO_OWNER, REPO_NAME);//Github API
    //gitee 确保小写
    getLatestGiteeVersion(REPO_OWNER.toLower(), REPO_NAME.toLower()); //Gitee API
}

void VersionChecker::getLatestGiteeVersion(const QString &owner, const QString &repo) {
    // 构建 Gitee API 请求 URL，格式与 GitHub 类似
    QString url = QString("https://gitee.com/api/v5/repos/%1/%2/releases/latest")
            .arg(owner, repo);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    const QString header = "APP" + DataManager::instance().const_config_data.name + "_" + DataManager::instance().
            const_config_data.version;
    request.setHeader(QNetworkRequest::UserAgentHeader, header);

    // 设置超时（通过定时器实现）
    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(10000); // 10秒超时

    QNetworkReply *reply = m_networkManager->get(request);

    // 连接超时处理
    connect(timeoutTimer, &QTimer::timeout, this, [reply, timeoutTimer, this]() {
        if (reply && reply->isRunning()) {
            reply->abort();
            timeoutTimer->deleteLater();
            emit errorOccurred("Request Timeout: 10 seconds have not received a response");
        }
    });

    // 请求完成时删除定时器
    connect(reply, &QNetworkReply::finished, timeoutTimer, &QTimer::deleteLater);
}

void VersionChecker::getLatestGithubVersion(const QString &owner, const QString &repo) {
    QString url = QString("https://api.github.com/repos/%1/%2/releases/latest")
            .arg(owner, repo);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    const QString header = "APP" + DataManager::instance().const_config_data.name + "_" + DataManager::instance().
            const_config_data.version;
    request.setHeader(QNetworkRequest::UserAgentHeader, header);

    // 设置超时（通过定时器实现）
    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(10000); // 10秒超时

    QNetworkReply *reply = m_networkManager->get(request);

    // 连接超时处理
    connect(timeoutTimer, &QTimer::timeout, this, [reply, timeoutTimer, this]() {
        if (reply && reply->isRunning()) {
            reply->abort();
            timeoutTimer->deleteLater();
            emit errorOccurred("Request Timeout: 10 seconds have not received a response");
        }
    });

    // 请求完成时删除定时器
    connect(reply, &QNetworkReply::finished, timeoutTimer, &QTimer::deleteLater);
}

void VersionChecker::onReplyFinished(QNetworkReply *reply) {
    // 确保reply被正确删除
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg;
        if (reply->error() == QNetworkReply::OperationCanceledError) {
            errorMsg = "Request Timeout or Canceled";
        } else {
            errorMsg = QString("GitHub API Error: %1").arg(reply->errorString());
        }

        qWarning() << "⚠️ Check Version Error:" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    // 读取响应数据
    QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = QString("JSON Parse Error: %1").arg(parseError.errorString());
        qWarning() << "⚠️ JSON Parse Error:" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    if (!jsonDoc.isObject()) {
        QString errorMsg = "Invalid JSON Document";
        qWarning() << "⚠️" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }


    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("tag_name")) {
        QString errorMsg = "No version number found in JSON document";
        qWarning() << "⚠️" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    if (reply->url().host().contains("gitee.com")) {
        // Gitee API 的 releases/latest 接口中，版本号字段也是 "tag_name"
        if (!jsonObj.contains("tag_name")) {
            QString errorMsg = "Gitee API not contains version: (tag_name)";
            qWarning() << "⚠️" << errorMsg;
            emit errorOccurred(errorMsg);
            return;
        }
    } else {
        if (!jsonObj.contains("tag_name")) {
            QString errorMsg = "GitHub API not contains version: (tag_name)";
            qWarning() << "⚠️" << errorMsg;
            emit errorOccurred(errorMsg);
            return;
        }
    }
    QString remoteVer = jsonObj["tag_name"].toString();


    if (remoteVer == m_localVersion) {
        QString message = QString("✅ Same Version \nVersion %1 is the latest release")
                .arg(m_localVersion);
        qDebug() << message;
        emit versionCheckCompleted(true, message);
    } else {
        QString message = QString("❌ Version Different \nLatest release: %1 \nLocal Version: %2")
                .arg(remoteVer, m_localVersion);
        qDebug() << message;
        emit versionCheckCompleted(false, message);
    }
}
