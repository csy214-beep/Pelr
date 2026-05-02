#include "translator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>

Translator::Translator(QObject *parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this)),
      m_baseUrl("http://localhost:9140")
{
}

Translator *Translator::instance()
{
    static Translator inst;
    return &inst;
}

void Translator::setBaseUrl(const QString &url)
{
    m_baseUrl = url;
}

QNetworkReply *Translator::postJson(const QString &endpoint, const QByteArray &data)
{
    QNetworkRequest request(QUrl(m_baseUrl + endpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return m_manager->post(request, data);
}

void Translator::translate(const QString &text,
                           const QString &targetLang,
                           const QString &provider)
{
    QJsonObject jsonBody;
    jsonBody["text"] = text;
    jsonBody["target_lang"] = targetLang;
    jsonBody["provider"] = provider;

    QJsonDocument doc(jsonBody);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = postJson("/translate", postData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
            {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit translationError(reply->errorString());
            return;
        }

        QByteArray responseData = reply->readAll();
        QJsonDocument respDoc = QJsonDocument::fromJson(responseData);
        if (!respDoc.isObject()) {
            emit translationError("Invalid JSON response");
            return;
        }

        QJsonObject respObj = respDoc.object();
        if (respObj.contains("translated_text")) {
            QString result = respObj.value("translated_text").toString();
            emit translationFinished(result);
        } else if (respObj.contains("error")) {
            QString errMsg = respObj.value("error").toString();
            emit translationError(errMsg);
        } else {
            emit translationError("Unknown response format");
        } });
}