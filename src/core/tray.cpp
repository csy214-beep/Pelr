/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "tray.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QTime>
#include <QDebug>
#include <QFile>
#include "launcher.hpp"
#include "data.hpp"
#include "launcherMenu.hpp"
#include "custommenu.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <iostream>
// 定义 IAudioMeterInformation 接口的 GUID
static const IID IID_IAudioMeterInformation =
        {0xC02216F6, 0x8C67, 0x4B5B, {0x9D, 0x00, 0xD0, 0x08, 0xE7, 0x3E, 0x00, 0x64}};

// 手动定义 IAudioMeterInformation 接口
class IAudioMeterInformation : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE GetPeakValue(float *pfPeak) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetMeteringChannelCount(UINT32 *pnChannelCount) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetChannelsPeakValues(UINT32 u32ChannelCount, float *afPeakValues) = 0;

    virtual HRESULT STDMETHODCALLTYPE QueryHardwareSupport(DWORD *pdwHardwareSupportMask) = 0;
};

class TrayIcon::AudioActivityDetector {
private:
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioMeterInformation *pMeterInfo = nullptr;

public:
    ~AudioActivityDetector() {
        Cleanup();
    }

    bool Initialize() {
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
            return false;
        }

        // 创建设备枚举器
        hr = CoCreateInstance(
            __uuidof(MMDeviceEnumerator), NULL,
            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void **) &pEnumerator);
        if (FAILED(hr)) return false;

        // 获取默认音频端点
        hr = pEnumerator->GetDefaultAudioEndpoint(
            eRender, eConsole, &pDevice);
        if (FAILED(hr)) return false;

        // 激活音频计量信息接口
        hr = pDevice->Activate(
            IID_IAudioMeterInformation, CLSCTX_ALL,
            NULL, (void **) &pMeterInfo);

        return SUCCEEDED(hr);
    }

    // 检测是否有音频活动（返回峰值电平）
    float GetPeakValue() {
        if (!pMeterInfo) return 0.0f;

        float peak = 0.0f;
        HRESULT hr = pMeterInfo->GetPeakValue(&peak);
        if (SUCCEEDED(hr)) {
            return peak;
        }
        return 0.0f;
    }

    // 检测是否正在播放音频
    bool IsAudioPlaying(float threshold = 0.01f) {
        return GetPeakValue() > threshold;
    }

private:
    void Cleanup() {
        if (pMeterInfo) {
            pMeterInfo->Release();
            pMeterInfo = nullptr;
        }
        if (pDevice) {
            pDevice->Release();
            pDevice = nullptr;
        }
        if (pEnumerator) {
            pEnumerator->Release();
            pEnumerator = nullptr;
        }
        CoUninitialize();
    }
};

// 初始化静态成员变量
TrayIcon *TrayIcon::m_instance = nullptr;

TrayIcon *TrayIcon::instance() {
    if (!m_instance) {
        m_instance = new TrayIcon();
    }
    return m_instance;
}

void TrayIcon::showMessage(const QString &title, const QString &msg,
                           QSystemTrayIcon::MessageIcon icon, int timeout) {
    if (m_instance && !m_instance->m_silentMode) {
        m_instance->QSystemTrayIcon::showMessage(title, msg, icon, timeout);
    }
}

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent) {
    setIcon(m_appIcon);
    setToolTip(
        DataManager::instance().const_config_data.name + " "
        + DataManager::instance().const_config_data.version
    );

    m_rotateTimer = new QTimer(this);
    connect(m_rotateTimer, &QTimer::timeout, this, &TrayIcon::rotateNote);

    m_audioCheckTimer = new QTimer(this);
    connect(m_audioCheckTimer, &QTimer::timeout, this, &TrayIcon::checkAudioActivity);
    m_audioCheckTimer->setInterval(500);

    // 创建右键菜单
    menu = new CustomMenu();

    action_resetWinLoc = new QAction(tr("重置位置"), this);
    action_showWin = new QAction(tr("显示界面"), this);
    action_silentMode = new QAction(tr("静默模式"), this);
    action_switchDrag = new QAction(tr("锁定位置"), this);
    action_mediaPlayer = new QAction(tr("播放媒体"), this);
    QAction *action_openPath = new QAction(tr("打开目录"), this);
    action_quit = new QAction(tr("退出程序"), this);
    action_keyListener = new QAction(tr("按键监听"), this);

    // 连接信号和槽（保持原有连接不变）
    connect(action_openPath, &QAction::triggered, []() {
        QString appDir = QCoreApplication::applicationDirPath();
        launchByPath(appDir);
    });

    connect(action_silentMode, &QAction::triggered, [this]() {
        m_silentMode = !m_silentMode;
        switchText(action_silentMode);
    });

    connect(action_switchDrag, &QAction::triggered, [this]() {
        switchText(action_switchDrag);
    });

    QAction *action_startApp = new QAction("启动项目", this);
    action_startApp->setMenu(launcherMenu::instance());
    // 如果有内容就添加到菜单
    if (launcherMenu::instance()->hasContent) {
        // 添加菜单项到菜单
        menu->addAction(action_startApp);
        menu->addSeparator();
    }

    menu->addActions({
        action_resetWinLoc, action_silentMode, action_switchDrag,
        action_keyListener,
        action_showWin, action_mediaPlayer, action_openPath
    });
    menu->addSeparator();
    menu->addAction(action_quit);

    // 设置托盘图标的菜单
    this->setContextMenu(menu);

    qDebug() << "TrayIcon singleton initialized";
    // 显示托盘图标
    this->show();
    switchMusicIcon(DataManager::instance().getBasicData().isMusicIcon);
}

void TrayIcon::switchText(QAction *action) {
    QString text = action->text();
    if (text.contains("* ")) {
        action->setText(text.replace("* ", ""));
    } else {
        action->setText("* " + text);
    }
}

void TrayIcon::switchText(QAction *action, bool flag) {
    QString text = action->text();
    const bool st = text.contains("* ");
    if (st && !flag) {
        action->setText(text.replace("* ", ""));
    }
    if (!st && flag) {
        action->setText("* " + text);
    }
}

void TrayIcon::initializeAudioDetector() {
    m_audioDetector = new AudioActivityDetector();
    if (m_audioDetector->Initialize()) {
        m_audioCheckTimer->start();
        qDebug() << "Audio detector initialized";
    } else {
        qDebug() << "Audio detector initialization failed";
    }
}

QPixmap TrayIcon::createMusicIcon() const {
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 应用旋转
    if (m_rotating) {
        painter.translate(32, 32);
        painter.rotate(m_angle);
        painter.translate(-32, -32);
    }

    // 设置字体和颜色
    QFont font(DataManager::instance()._font);
    font.setPointSize(64);
    font.setBold(true);
    painter.setFont(font);

    // 根据旋转状态改变颜色
    if (m_rotating) {
        painter.setPen(Qt::red); // 有音频
    } else {
        painter.setPen(Qt::white); // 白色表示无音频
    }

    // 绘制音乐符号
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "♫");

    return pixmap;
}

void TrayIcon::rotateNote() {
    m_angle = (m_angle + 30) % 360;
    setIcon(QIcon(createMusicIcon()));
}

void TrayIcon::checkAudioActivity() {
    if (!m_audioDetector) return;

    bool isAudioPlaying = m_audioDetector->IsAudioPlaying();
    // 根据音频状态控制旋转
    if (isAudioPlaying && !m_rotating) {
        // 开始旋转
        m_rotating = true;
        m_rotateTimer->start(100); // 固定速度：100ms
    } else if (!isAudioPlaying && m_rotating) {
        // 停止旋转
        m_rotating = false;
        m_rotateTimer->stop();
    }

    // 更新图标以反映颜色变化
    setIcon(QIcon(createMusicIcon()));
}

void TrayIcon::switchMusicIcon(const bool flag) {
    if (flag) {
        // flag = true; 启用旋转
        initializeAudioDetector();
        qDebug() << "music icon enabled";
    } else {
        // flag = false; 禁用旋转
        m_audioCheckTimer->stop();
        m_rotateTimer->stop();
        m_rotating = false;
        setIcon(m_appIcon);
        qDebug() << "music icon disabled";
    }
}

TrayIcon::~TrayIcon() {
    delete menu;
    m_instance = nullptr;
    delete m_audioDetector;
}
