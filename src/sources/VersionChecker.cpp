#include "VersionChecker.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "data.hpp"

void checkSslSupport() {
    qDebug() << "SSL 支持:" << QSslSocket::supportsSsl();
    qDebug() << "SSL 版本:" << QSslSocket::sslLibraryVersionString();
    qDebug() << "SSL 编译版本:" << QSslSocket::sslLibraryBuildVersionString();
}

VersionChecker::VersionChecker(QObject *parent)
    : QObject(parent)
      , m_networkManager(new QNetworkAccessManager(this)) {
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

    getLatestGithubVersion(REPO_OWNER, REPO_NAME);
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
            emit errorOccurred("请求超时: 10秒内未收到响应");
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
            errorMsg = "请求超时或被取消";
        } else {
            errorMsg = QString("GitHub API错误: %1").arg(reply->errorString());
        }

        qWarning() << "⚠️ 检查失败:" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    // 读取响应数据
    QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QString errorMsg = QString("JSON解析错误: %1").arg(parseError.errorString());
        qWarning() << "⚠️ JSON解析失败:" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    if (!jsonDoc.isObject()) {
        QString errorMsg = "无效的API响应格式";
        qWarning() << "⚠️" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("tag_name")) {
        QString errorMsg = "API响应中未找到版本号";
        qWarning() << "⚠️" << errorMsg;
        emit errorOccurred(errorMsg);
        return;
    }

    QString remoteVer = jsonObj["tag_name"].toString();

    if (remoteVer == m_localVersion) {
        QString message = QString("✅ 版本一致 \n当前版本 %1 已是最新版！")
                .arg(m_localVersion);
        qDebug() << message;
        emit versionCheckCompleted(true, message);
    } else {
        QString message = QString("❌ 版本不一致 \nGitHub 最新发布：%1 \n当前：%2")
                .arg(remoteVer, m_localVersion);
        qDebug() << message;
        emit versionCheckCompleted(false, message);
    }
}
