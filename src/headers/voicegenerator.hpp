/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
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

class VoiceGenerator : public QObject {
    Q_OBJECT

public:
    static VoiceGenerator *instance() {
        static VoiceGenerator instance;
        return &instance;
    }

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

    void handleGenerateResponse(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject json = doc.object();
            QString filePath = json["file_path"].toString();
            if (!filePath.isEmpty() && QFile::exists(filePath)) {
                emit voiceGenerated(filePath);
            } else {
                QString error = json["error"].toString();
                emit errorOccurred(error.isEmpty() ? "File not found" : error);
            }
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    }

    QNetworkAccessManager *manager;
    QMediaPlayer *player;
};
