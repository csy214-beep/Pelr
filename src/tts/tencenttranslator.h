#ifndef TENCENTTRANSLATOR_H
#define TENCENTTRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include "data.hpp"

class TencentTranslator : public QObject
{
    Q_OBJECT

public:
    explicit TencentTranslator(QObject *parent = nullptr);

    void setCredentials(const QString &secretId,
                        const QString &secretKey,
                        const QString &region = QStringLiteral("ap-guangzhou"),
                        int projectId = 0);

    void translate(const QString &text,
                   const QString &targetLang,
                   const QString &sourceLang = QStringLiteral("auto"));

    /**
     * @brief 同步测试接口：给定 TTSConfig 和文本，结果写入 msg
     * @param config 包含腾讯云密钥/区域/语言的 TTSConfig
     * @param text   待翻译文本
     * @param msg    输出参数，调用者必须提供有效指针；成功存翻译文本，失败存错误信息
     */
    static void testTranslate(const TTSConfig &config,
                              const QString &text,
                              QString *msg = nullptr);

signals:
    void translationFinished(const QString &translatedText);
    void translationError(const QString &errorMessage);

private:
    QNetworkAccessManager *m_manager;

    QString m_secretId;
    QString m_secretKey;
    QString m_region;
    int m_projectId;

    // 腾讯云 API 3.0 签名 V3
    QByteArray sign(const QString &service,
                    const QString &action,
                    const QByteArray &payload,
                    const QDateTime &timestamp) const;

    QNetworkReply *sendRequest(const QByteArray &payload,
                               const QString &action,
                               const QDateTime &timestamp);

    QString parseResponse(const QByteArray &responseData, QString &errorMsg) const;
};

#endif // TENCENTTRANSLATOR_H