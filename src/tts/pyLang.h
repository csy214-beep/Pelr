#ifndef PYLANG_H
#define PYLANG_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QString>

class PyLang : public QObject
{
    Q_OBJECT

public:
    explicit PyLang(QObject *parent = nullptr);

    // 发起请求
    void fetchLanguages();
    void fetchProviders();

    // 测试翻译（新增）
    void testTranslation(const QString &text, const QString &targetLang, const QString &provider);

    // 设置服务器地址（默认 http://localhost:9140）
    void setBaseUrl(const QString &url);

signals:
    void languagesReady(const QList<QString> &langs);
    void providersReady(const QList<QString> &providers);
    void translationTestCompleted(bool success); // 翻译测试完成信号
    void errorOccurred(const QString &errorMessage);

private:
    QNetworkAccessManager *m_manager;
    QString m_baseUrl;

    // 通用 GET 请求
    QNetworkReply *get(const QString &endpoint);
    // 解析 JSON 数组字段
    static QList<QString> parseStringList(const QByteArray &json, const QString &key);
};

#endif

// 绑定结果信号
// connect(client, &TtsApiClient::languagesReady, this, [](const QList<QString> &langs) {
//     qDebug() << "Languages:" << langs;
// });
// connect(client, &TtsApiClient::providersReady, this, [](const QList<QString> &providers) {
//     qDebug() << "Providers:" << providers;
// });
// connect(client, &TtsApiClient::errorOccurred, this, [](const QString &err) {
//     qWarning() << "Error:" << err;
// });

// 连接测试结果信号
// connect(client, &PyLang::translationTestCompleted, this, [](bool success)
//         { qDebug() << "Translation test" << (success ? "succeeded" : "failed"); });
// connect(client, &PyLang::errorOccurred, this, [](const QString &err)
//         { qWarning() << "PyLang error:" << err; });