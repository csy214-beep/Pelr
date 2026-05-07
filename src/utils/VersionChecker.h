
#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QMap>
#include <QSet>

/**
 * @brief 单个检查源的结果信息
 */
struct SourceCheckResult
{
    QString sourceName; // "GitHub", "Gitee", "Pelr"
    QString url;        // 实际请求的 API URL
    bool success = false;
    QString errorString;
    QString latestVersion; // tag_name
    QString body;          // 发布说明 (Markdown)
    QString downloadUrl;   // 下载链接
    QString publishedAt;   // 发布时间
};

/**
 * @brief 版本检查汇总结果
 */
struct VersionCheckSummary
{
    QString localVersion;
    QList<SourceCheckResult> results;
    bool anySuccess = false;
    bool anyNewerVersion = false; // 是否有源版本高于本地
    QString newestRemoteVersion;  // 最高正式版版本号
};

class VersionChecker : public QObject
{
    Q_OBJECT

public:
    explicit VersionChecker(QObject *parent = nullptr);

    ~VersionChecker();

    /**
     * @brief 启动版本检查 (同时检查 GitHub、Gitee、Pelr)
     * @param localVersion 当前本地版本号 (例如 "20260405.12-dev")
     */
    void checkVersionMatch(const QString &localVersion);

    /**
     * @brief 比较两个版本号的大小
     * @param v1 版本号1
     * @param v2 版本号2
     * @return -1: v1 < v2, 0: 相等, 1: v1 > v2
     */
    static int compareVersions(const QString &v1, const QString &v2);

signals:
    /**
     * @brief 所有检查源均完成后的汇总结果信号
     */
    void versionCheckResult(const VersionCheckSummary &summary);

    /**
     * @brief 单个源发生错误时的通知 (可选)
     */
    void sourceError(const QString &source, const QString &error);

    // 兼容旧信号 (可不使用)
    void versionCheckCompleted(bool isMatch, const QString &message);

    void errorOccurred(const QString &errorMessage);

private slots:
    void onReplyFinished(QNetworkReply *reply);

    void onTimeout();

private:
    void checkByGithub(const QString &owner, const QString &repo);

    void checkByGitee(const QString &owner, const QString &repo);

    void checkByPelr(); // 自建 API

    void processReply(QNetworkReply *reply, const QString &sourceName);

    void finalizeIfAllFinished();

    QNetworkAccessManager *m_networkManager;
    QString m_localVersion;

    QSet<QNetworkReply *> m_pendingReplies;
    QMap<QNetworkReply *, QTimer *> m_timeoutTimers;
    QMap<QString, SourceCheckResult> m_results;
};

#endif // VERSIONCHECKER_H
