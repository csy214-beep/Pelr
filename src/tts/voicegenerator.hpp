/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QMediaPlayer>
#include "data.hpp"
#include "launcher.hpp"

class VoiceGenerator : public QObject {
    Q_OBJECT

public:
    static VoiceGenerator *instance() {
        static VoiceGenerator instance;
        return &instance;
    }

    // 原有讯飞 TTS 调用方式（保持不变）
    void generateVoice(const QString &appid, const QString &apiKey, const QString &apiSecret,
                       const QString &text, const QString &voice = "x4_yezi") {
        QString speaker = voice;
        if (speaker.isEmpty()) {
            speaker = "x4_yezi";
        }
        // 准备JSON数据
        QJsonObject json;
        json["appid"] = appid;
        json["api_key"] = apiKey;
        json["api_secret"] = apiSecret;
        json["text"] = text;
        json["voice"] = speaker;

        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        // 创建网络请求
        QNetworkRequest request(QUrl("http://localhost:9140/generate"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 发送POST请求
        QNetworkReply *reply = manager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleGenerateResponse(reply);
        });
    }

    // 新增：OpenAI 风格 TTS 调用（Edge TTS），支持发言人和语速
    // voice: 例如 "zh-CN-XiaoyiNeural", "nova" 等
    // speed: 0.25 ~ 4.0，默认 1.0
    void generateVoiceOpenAI(const QString &text, const QString &voice = "zh-CN-XiaoxiaoNeural",
                             double speed = 1.0) {
        // 参数校验
        double finalSpeed = qBound(0.25, speed, 4.0);

        // 准备JSON数据（符合 /v1/audio/speech_local 接口）
        QJsonObject json;
        json["input"] = text;
        json["voice"] = voice;
        json["response_format"] = "mp3";
        json["speed"] = finalSpeed;

        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        // 创建网络请求（注意端口与整合服务器一致，默认9140）
        QNetworkRequest request(QUrl("http://localhost:9140/v1/audio/speech_local"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 可选：如果需要 API Key 鉴权，取消下一行的注释并填写正确的 key
        // request.setRawHeader("Authorization", "Bearer your_api_key_here");

        QNetworkReply *reply = manager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleOpenAIResponse(reply);
        });
    }

    void playVoice(const QString &filePath) {
        if (player) {
            //volume
            player->setVolume(DataManager::instance().getBasicData().volume);
            // 在 Qt5 中使用 setMedia 而不是 setSource
            player->setMedia(QUrl::fromLocalFile(filePath));
            player->play();
        }
    }

    void stopVoice() {
        if (player) {
            player->stop();
        }
    }

signals:
    void voiceGenerated(const QString &filePath);

    void errorOccurred(const QString &errorMessage);

    void playbackFinished(); // 添加这个信号

private:
    VoiceGenerator(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);

        // 初始化音频播放器
        player = new QMediaPlayer(this);
        player->setVolume(50);

        // 连接播放器信号 - 使用 Qt5 的 API
        connect(player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
            if (state == QMediaPlayer::StoppedState) {
                emit playbackFinished();
            }
        });
    }

    VoiceGenerator(const VoiceGenerator &) = delete;

    VoiceGenerator &operator=(const VoiceGenerator &) = delete;

    // 处理讯飞 TTS 的响应（原有逻辑）
    void handleGenerateResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject json = doc.object();
            qDebug() << "voice generate response:" << json;
            QString filePath = json["file_path"].toString();
            if (!filePath.isEmpty() && QFile::exists(filePath)) {
                emit voiceGenerated(filePath);
            } else {
                QString error = json["error"].toString();
                emit errorOccurred(error.isEmpty() ? "File not found" : error);
            }
        } else {
            if (reply->error() == QNetworkReply::ConnectionRefusedError) {
                onConnectionRefused();
            }
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    }

    // 处理 OpenAI 风格 TTS 的响应（新逻辑）
    void handleOpenAIResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject json = doc.object();
            qDebug() << "OpenAI TTS response:" << json;
            QString filePath = json["file_path"].toString();
            if (!filePath.isEmpty() && QFile::exists(filePath)) {
                emit voiceGenerated(filePath);
            } else {
                QString error = json["error"].toString();
                emit errorOccurred(error.isEmpty() ? "File not found from OpenAI TTS" : error);
            }
        } else {
            if (reply->error() == QNetworkReply::ConnectionRefusedError) {
                onConnectionRefused();
            }
            emit errorOccurred(QString("OpenAI TTS error: %1").arg(reply->errorString()));
        }
        reply->deleteLater();
    }

    void onConnectionRefused() {
        if (!DataManager::instance().getTTSConfig().isRunTTSServerOnStartUp)
            return;
        qDebug() << "TTS server refused connection, trying to launch it...";
        launchByPath(DataManager::instance().const_config_data.tts_server);
    }

    QNetworkAccessManager *manager;
    QMediaPlayer *player;
};
