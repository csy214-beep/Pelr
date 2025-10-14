/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#include "BubbleBox.h"
#include  <QCoreApplication>
#include  <QApplication>
#include <QFile>
#include  <QScreen>
#include  <QTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <ctime>
#include <qmediaplayer.h>
#include <QPainter>
#include <random>
#include <QSizePolicy>
#include "adjustLabel.hpp"
#include <QUrl>
#include <QThread>
#include "voicegenerator.hpp"
#include  "data.hpp"
#include <QString>
#include <QColor>
// 时间段语录文件
#define TIMETEXT_FILE ":/assets/text/timeText.json"
#define DAILYTEXT_FILE ":/assets/text/dailyText.json"

// 初始化静态成员变量
BubbleBox *BubbleBox::m_instance = nullptr;

BubbleBox *BubbleBox::instance(QLabel *parent) {
    if (!m_instance) {
        m_instance = new BubbleBox(parent);
    }
    return m_instance;
}

BubbleBox::BubbleBox(QLabel *parent) : QLabel(parent) {
    if (DataManager::instance().getBasicData().isTop) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowTransparentForInput | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowTransparentForInput);
    }
    int max_width = 300 * DataManager::instance().getBasicData().model_size / 150;
    int max_height = 500 * DataManager::instance().getBasicData().model_size / 150;
    setMaximumSize(max_width, max_height);
    // 设置透明背景属性
    setAttribute(Qt::WA_TranslucentBackground);
    setAlignment(Qt::AlignCenter);
    QFont font = DataManager::instance()._font;
    font.setPointSize((int) 12 * (DataManager::instance().getBasicData().model_size / 150));
    font.setWeight(50);
    // font.setItalic(true);
    setFont(font);
    QString foreColor = DataManager::instance().getBasicData().color_bubble.second;
    if (foreColor.isEmpty()) {
        foreColor = "rgba(255, 255, 255, 0.8)";
    }
    setStyleSheet(
        "color: " + foreColor + ";"
        "border-radius:20px;"
        "padding:8px;"
    );
    setWordWrap(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    adjustSize();

    // 连接信号
    connect(VoiceGenerator::instance(), &VoiceGenerator::voiceGenerated,
            this, [&](const QString &filePath) {
                qDebug() << "Voice generated:" << filePath;
                // 可以直接播放
                VoiceGenerator::instance()->playVoice(filePath);
                show();
                resetFadeTimer();
            });

    connect(VoiceGenerator::instance(), &VoiceGenerator::errorOccurred,
            this, [&](const QString &error) {
                qDebug() << "Error:" << error;
                show();
                resetFadeTimer();
            });

    fadeTimer = new QTimer(this);
    fadeTimer->setSingleShot(true);
    fadeTimer->setInterval(8000); //8s
    connect(fadeTimer, &QTimer::timeout, this, &BubbleBox::hide);
    hide();

    // 初始化当前时间
    now = "";
    showTime();
}

void BubbleBox::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    QString color = DataManager::instance().getBasicData().color_bubble.first;
    QColor backgroundColor(color);
    backgroundColor = !backgroundColor.isValid() ? QColor(37, 153, 236, 219) : backgroundColor;
    // 绘制半透明圆角矩形背景
    QRect rect = this->rect();
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 20, 20);

    // 调用基类的paintEvent来绘制文本
    QLabel::paintEvent(event);
}

void BubbleBox::RandomSentence() {
    QString keyName = "daily";
    // 打开文件
    QFile file(DAILYTEXT_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << file.errorString();
        textSet("今天也是美好的一天呢~");
        return;
    }
    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();
    // 解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "解析JSON失败";
        textSet("今天也是美好的一天呢~");
        return;
    }
    // 获取根对象
    QJsonObject root = doc.object();

    // 检查key是否存在
    if (!root.contains(keyName) || !root[keyName].isArray()) {
        qWarning() << "JSON格式错误或缺少'daily'键";
        textSet("今天也是美好的一天呢~");
        return;
    }

    // 获取数组
    QJsonArray targetArray = root[keyName].toArray();
    if (targetArray.isEmpty()) {
        textSet("今天也是美好的一天呢~");
        return;
    }

    int index = std::rand() % targetArray.size();
    textSet(targetArray[index].toString());
}

QString BubbleBox::getPeriodText() {
    QTime currentTime = QTime::currentTime();
    int hour = currentTime.hour();
    QString keyName;

    if (hour >= 6 && hour < 10) {
        keyName = "清晨";
    } else if (hour >= 10 && hour < 12) {
        keyName = "上午";
    } else if (hour >= 12 && hour < 14) {
        keyName = "午间";
    } else if (hour >= 14 && hour < 18) {
        keyName = "下午";
    } else if (hour >= 18 && hour < 20) {
        keyName = "傍晚";
    } else {
        keyName = "深夜";
    }

    // 打开文件
    QFile file(TIMETEXT_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << file.errorString();
        return "你好呀~";
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "解析JSON失败";
        return "你好呀~";
    }

    // 获取根对象
    QJsonObject root = doc.object();

    // 检查key是否存在
    if (!root.contains(keyName) || !root[keyName].isArray()) {
        qWarning() << "JSON格式错误或缺少时段键";
        return "你好呀~";
    }

    // 获取数组
    QJsonArray targetArray = root[keyName].toArray();
    if (targetArray.isEmpty()) {
        return "你好呀~";
    }

    int index = std::rand() % targetArray.size();
    return targetArray[index].toString();
}

void BubbleBox::showTime() {
    QTime currentTime = QTime::currentTime();
    QString time = currentTime.toString("hh:mm");
    QString period = getPeriodText(); // 获取当前时间段句子
    if (isFirst) {
        textSet(period + "\n现在是" + time + "哦~");
        isFirst = false;
    } else if ((time.contains(":00") || time.contains(":30")) && time != this->now) {
        textSet(period + "\n现在是" + time + "哦~");
        this->now = time;
    }
}

QString BubbleBox::GetSystemTime() {
    QTime currentTime = QTime::currentTime();
    return currentTime.toString("hh:mm");
}

void BubbleBox::setThinkingText() {
    fadeTimer->stop();
    setText("In response...");
    adjustSize();
    show();
}


void BubbleBox::textSet(const QString &text) {
    if (!DataManager::instance().getBasicData().isSaying) {
        qDebug() << "不使用语音合成接口";
        setText(text);
        qInfo() << "BubbleBox:" << text; // 日志记录
        adjustSize();
        show();
        resetFadeTimer();
        return;
    }
    QString APPID = DataManager::instance().getBasicData().APPID;
    QString APIKey = DataManager::instance().getBasicData().APIKey;
    QString APISecret = DataManager::instance().getBasicData().APISecret;
    QString voice = DataManager::instance().getBasicData().speaker;
    if (!APPID.isEmpty() && !APIKey.isEmpty() && !APISecret.isEmpty()) {
        qDebug() << "TTS 配置完整，调用语音合成接口";
        VoiceGenerator::instance()->generateVoice(APPID, APIKey, APISecret, text, voice);
        setText(text);
        adjustSize();
    } else {
        qDebug() << "TTS 配置不完整，不使用语音合成接口";
        setText(text);
        adjustSize();
        show();
        resetFadeTimer();
    }
}

void BubbleBox::resetFadeTimer() {
    fadeTimer->stop();
    fadeTimer->start();
}

void BubbleBox::updateWindowLocation(int f_x, int f_y, int f_w, int f_h) {
    QCoreApplication *coreapp = QCoreApplication::instance();
    QApplication *app = qobject_cast<QApplication *>(coreapp);
    if (!app) return;

    // 计算初始位置
    int x = f_x - f_w / 8;
    int y = f_y;

    // 位置未改变则不更新
    if (this->x() == x && this->y() == y) return;

    // 获取目标位置所在的屏幕
    QScreen *targetScreen = app->screenAt(QPoint(x, y));
    if (!targetScreen) {
        // 如果没有找到屏幕，使用主屏幕作为备选
        targetScreen = app->primaryScreen();
        if (!targetScreen) return;
    }

    // 获取屏幕的可用几何区域（考虑任务栏等系统UI）
    QRect screenGeometry = targetScreen->availableGeometry();

    // 窗口宽度
    int winWidth = width();
    int screenWidth = screenGeometry.width();
    int screenLeft = screenGeometry.left();

    // 水平边界检测
    if (x < screenLeft) {
        // 窗口左侧超出屏幕左边界
        x = x + f_w;
    } else if (x + winWidth > screenLeft + screenWidth) {
        // 窗口右侧超出屏幕右边界
        if (winWidth <= screenWidth) {
            // 窗口宽度小于等于屏幕宽度，可以完全放入屏幕
            x = screenLeft + screenWidth - winWidth;
        } else {
            // 窗口宽度大于屏幕宽度，只能尽可能显示
            x = screenLeft;
        }
    }

    // 垂直边界检测
    if (y < screenGeometry.top()) {
        y = screenGeometry.top();
    } else if (y + height() > screenGeometry.bottom()) {
        y = screenGeometry.bottom() - height();
    }

    move(x, y);
}

BubbleBox::~BubbleBox() {
    // 清理静态实例指针
    m_instance = nullptr;
}
