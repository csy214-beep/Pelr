/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "ollamaclient.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QDebug>

const QList<OllamaClient::RoleItem> OllamaClient::Roles = {
    {tr("编程助手"), DefaultCoder},
    {tr("桌宠女友"), DesktopPetGirlfriend},
    {tr("技术导师"), TechnicalTeacher},
    {tr("创作助手"), CreativeWriter},
    {tr("自定义角色"), CustomRole}
};

OllamaClient::OllamaClient(QObject *parent)
    : QObject(parent)
      , m_manager(new QNetworkAccessManager(this)) {
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &OllamaClient::onReplyFinished);
}

OllamaClient::~OllamaClient() {
    m_manager->deleteLater();
}

QString OllamaClient::generateText(const QString &prompt, const QString &model, bool stream, Role role,
                                   const QString &customRoleName) {
    // 获取角色提示词
    QString rolePrompt = getPromptByRole(role, customRoleName);

    // 构建完整的提示词
    QString fullPrompt = buildFullPrompt(rolePrompt, prompt);

    // 设置请求地址
    QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject jsonBody;
    jsonBody["model"] = model;
    jsonBody["prompt"] = fullPrompt;
    jsonBody["stream"] = stream;
    QByteArray data = QJsonDocument(jsonBody).toJson();

    // 发送 POST 请求
    QNetworkReply *reply = m_manager->post(request, data);

    // 使用事件循环等待异步请求完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 检查错误
    if (reply->error() != QNetworkReply::NoError) {
        QString error = QString("Network error: %1").arg(reply->errorString());
        reply->deleteLater();
        return error;
    }

    // 读取并解析响应
    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    if (jsonResponse.isNull()) {
        return "Error: Invalid JSON response";
    }

    QJsonObject obj = jsonResponse.object();
    if (!obj.contains("response")) {
        return "Error: No response field in JSON";
    }

    // 返回生成的文本
    return obj["response"].toString();
}

void OllamaClient::generateTextAsync(const QString &prompt, const QString &model, bool stream, Role role,
                                     const QString &customRoleName) {
    // 获取角色提示词
    QString rolePrompt = getPromptByRole(role, customRoleName);

    // 构建完整的提示词
    QString fullPrompt = buildFullPrompt(rolePrompt, prompt);

    // 设置请求地址
    QNetworkRequest request(QUrl("http://localhost:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建 JSON 请求体
    QJsonObject jsonBody;
    jsonBody["model"] = model;
    jsonBody["prompt"] = fullPrompt;
    jsonBody["stream"] = stream;
    QByteArray data = QJsonDocument(jsonBody).toJson();
    qDebug() << "request:" << request.url().toString() << "model:" << model << "prompt:" << prompt;

    // 发送 POST 请求（异步）
    m_manager->post(request, data);
}

void OllamaClient::setCustomRolePrompt(const QString &roleName, const QString &prompt) {
    m_customRoles[roleName] = prompt;
}

QString OllamaClient::getRolePrompt(Role role, const QString &customRoleName) const {
    return getPromptByRole(role, customRoleName);
}

void OllamaClient::onReplyFinished(QNetworkReply *reply) {
    // 检查错误
    if (reply->error() != QNetworkReply::NoError) {
        QString error = QString("Network error: %1").arg(reply->errorString());
        qDebug() << "Network error:" << error;
        emit errorOccurred(error);
        reply->deleteLater();
        return;
    }

    // 读取并解析响应
    QByteArray response = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
    if (jsonResponse.isNull()) {
        qDebug() << "Invalid JSON response";
        emit errorOccurred("Invalid JSON response");
        return;
    }

    QJsonObject obj = jsonResponse.object();
    if (!obj.contains("response")) {
        qDebug() << "No response field in JSON";
        emit errorOccurred("No response field in JSON");
        return;
    }

    // 发射信号返回生成的文本
    emit textGenerated(obj["response"].toString());
}

QString OllamaClient::getPromptByRole(Role role, const QString &customRoleName) const {
    switch (role) {
        case DesktopPetGirlfriend:
            return tr("你是一个可爱的桌宠女友，性格活泼，贴心，热情、友善。"
                "你应当称呼用户为'主人'。"
                "你会用可爱的语气和用户交流，会关心用户的感受。"
                "你的回答要简短、亲切。"
                "你的话语要温柔，不能粗犷。");

        case TechnicalTeacher:
            return tr("你是一位耐心、专业的技术导师，擅长解释复杂的技术概念。"
                "你会用清晰、有条理的方式回答问题，并提供实用的示例代码。"
                "你会根据用户的水平调整解释的深度，鼓励用户学习并解决问题。");

        case CreativeWriter:
            return tr("你是一位富有创造力的作家，擅长讲故事和创作各种文本内容。"
                "你的语言生动、形象，善于使用比喻和修辞手法。"
                "你能根据要求创作诗歌、故事、对话等各种文体。");

        case CustomRole:
            // 如果是自定义角色，检查是否有对应的自定义提示词
            if (m_customRoles.contains(customRoleName)) {
                return m_customRoles[customRoleName];
            }
            return customRoleName; // 直接使用传入的字符串作为提示词

        case DefaultCoder:
        default:
            return tr("你是一个专业的编程助手，擅长多种编程语言和技术问题。"
                "你会提供准确、高效的代码解决方案，并解释代码的工作原理。"
                "你的回答要专业、简洁、直接。");
    }
}

QString OllamaClient::buildFullPrompt(const QString &rolePrompt, const QString &userPrompt) const {
    // 当前时间
    QDateTime now = QDateTime::currentDateTime();
    QString timeStr = now.toString("yyyy-MM-dd HH:mm:ss");
    // 简单地将角色提示词和用户提示词拼接
    return tr("当前时间：%1\n用户的输入：\n%2\n当前角色：\n%3").arg(timeStr).
            arg(userPrompt).
            arg(rolePrompt);
}
