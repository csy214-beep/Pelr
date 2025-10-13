#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class VersionChecker : public QObject {
    Q_OBJECT

public:
    explicit VersionChecker(QObject *parent = nullptr);

    ~VersionChecker();

    // 检查版本匹配的主函数
    void checkVersionMatch(const QString &localVersion);

signals:
    // 版本检查完成信号
    void versionCheckCompleted(bool isMatch, const QString &message);

    // 错误信号
    void errorOccurred(const QString &errorMessage);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    // 从GitHub API获取最新Release版本号
    void getLatestGithubVersion(const QString &owner, const QString &repo);

    // Gitee检查函数
    void getLatestGiteeVersion(const QString &owner, const QString &repo);

    QNetworkAccessManager *m_networkManager;
    QString m_localVersion;
};

#endif // VERSIONCHECKER_H
