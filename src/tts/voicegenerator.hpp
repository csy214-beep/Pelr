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
#include <QDebug>
#include "data.hpp"
#include "launcher.hpp"
#include "translator.h"

class VoiceGenerator : public QObject {
    Q_OBJECT

public:
    static VoiceGenerator *instance() {
        static VoiceGenerator instance;
        return &instance;
    }

    /**
     * @brief 主语音生成入口，根据配置自动处理翻译（如果需要）然后TTS
     * @param config TTS及翻译配置（不再需要包含 text）
     * @param text   待合成（或待翻译）的原始文本
     */
    void generateVoice(const TTSConfig &config, const QString &text)
    {
        // 判断是否需要翻译
        if (!config.tr_provider.isEmpty() && !config.tr_lang.isEmpty())
        {
            // 暂存配置和原文，翻译完成后再生成
            m_pendingConfig = config;
            m_pendingText = text;
            Translator::instance()->translate(text, config.tr_lang, config.tr_provider);
        }
        else
        {
            // 不需要翻译，直接生成
            doGenerateVoice(config, text);
        }
    }

    // 原有讯飞 TTS 调用方式（保持不变）
    void generateVoiceIFlytek(const QString &appid, const QString &apiKey, const QString &apiSecret,
                              const QString &text, const QString &voice = "x4_yezi")
    {
        QString speaker = voice;
        if (speaker.isEmpty()) {
            speaker = "x4_yezi";
        }
        QJsonObject json;
        json["appid"] = appid;
        json["api_key"] = apiKey;
        json["api_secret"] = apiSecret;
        json["text"] = text;
        json["voice"] = speaker;

        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkRequest request(QUrl("http://localhost:9140/generate"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = m_manager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleGenerateResponse(reply);
        });
    }

    // 新增：OpenAI 风格 TTS 调用（Edge TTS）
    void generateVoiceOpenAI(const QString &text, const QString &voice = "zh-CN-XiaoxiaoNeural",
                             double speed = 1.0)
    {
        double finalSpeed = qBound(0.25, speed, 4.0);

        QJsonObject json;
        json["input"] = text;
        json["voice"] = voice;
        json["response_format"] = "mp3";
        json["speed"] = finalSpeed;

        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkRequest request(QUrl("http://localhost:9140/v1/audio/speech_local"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = m_manager->post(request, data);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleOpenAIResponse(reply);
        });
    }

    void playVoice(const QString &filePath) {
        if (m_player)
        {
            m_player->setVolume(DataManager::instance().getBasicData().volume);
            m_player->setMedia(QUrl::fromLocalFile(filePath));
            m_player->play();
        }
    }

    void stopVoice() {
        if (m_player)
        {
            m_player->stop();
        }
    }

signals:
    void voiceGenerated(const QString &filePath);
    void errorOccurred(const QString &errorMessage);
    void playbackFinished();

private slots:
    // 翻译成功后的处理
    void onTranslationFinished(const QString &translatedText)
    {
        qDebug() << "Translation successful:" << translatedText;
        // 使用翻译后的文本生成语音
        doGenerateVoice(m_pendingConfig, translatedText);
    }

    void onTranslationError(const QString &errorMessage)
    {
        qWarning() << "Translation failed:" << errorMessage;
        // 回退到原文生成语音，并通知调用方
        emit errorOccurred("Translation failed: " + errorMessage);
        doGenerateVoice(m_pendingConfig, m_pendingText);
    }

private:
    VoiceGenerator(QObject *parent = nullptr) : QObject(parent) {
        m_manager = new QNetworkAccessManager(this);

        m_player = new QMediaPlayer(this);
        m_player->setVolume(50);

        connect(m_player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state)
                {
            if (state == QMediaPlayer::StoppedState) {
                emit playbackFinished();
            } });

        // 连接翻译信号
        Translator *trans = Translator::instance();
        connect(trans, &Translator::translationFinished, this, &VoiceGenerator::onTranslationFinished);
        connect(trans, &Translator::translationError, this, &VoiceGenerator::onTranslationError);
    }

    VoiceGenerator(const VoiceGenerator &) = delete;
    VoiceGenerator &operator=(const VoiceGenerator &) = delete;

    // 统一执行 TTS 生成
    void doGenerateVoice(const TTSConfig &config, const QString &text)
    {
        if (config.provider == 0)
        {
            generateVoiceOpenAI(text, config.speaker_openai_edge_tts, config.speed_openai_edge_tts);
        }
        else if (config.provider == 1)
        {
            generateVoiceIFlytek(config.iFlytek_APPID, config.iFlytek_APIKey, config.iFlytek_APISecret,
                                 text, config.iFlytek_speaker);
        }
        else
        {
            emit errorOccurred("Unknown TTS provider");
        }
    }

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

    QNetworkAccessManager *m_manager;
    QMediaPlayer *m_player;
    TTSConfig m_pendingConfig; // 暂存等待翻译完成的配置
    QString m_pendingText;     // 暂存待翻译的原始文本
};