#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

/**
 * @brief 单例翻译客户端，调用 Python TTS 服务的 /translate 接口
 */
class Translator : public QObject
{
    Q_OBJECT

public:
    static Translator *instance();

    void setBaseUrl(const QString &url);

    /**
     * @brief 异步执行翻译
     * @param text        待翻译文本
     * @param targetLang  目标语言代码（如 "en", "zh-Hans"）
     * @param provider    供应商名称（如 "google", "baidu"）
     */
    void translate(const QString &text,
                   const QString &targetLang,
                   const QString &provider);

signals:
    void translationFinished(const QString &translatedText);
    void translationError(const QString &errorMessage);

private:
    explicit Translator(QObject *parent = nullptr);
    ~Translator() override = default;

    Translator(const Translator &) = delete;
    Translator &operator=(const Translator &) = delete;

    QNetworkAccessManager *m_manager;
    QString m_baseUrl;

    QNetworkReply *postJson(const QString &endpoint, const QByteArray &data);
};

#endif // TRANSLATOR_H