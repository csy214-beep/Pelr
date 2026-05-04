/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include <QtWidgets/QApplication>
#include  <QCoreApplication>
#include "GLCore.h"
#include "tray.h"
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <QString>
#include <QDir>
#include <QStyleFactory>
#include <QString>
#include  "CheckApplication.h"
#include "TranslationManager.h"
#include "logger.hpp"
#include "NotificationWidget.h"
#include "initFileSys.h"
#include "voicevox_tts.h"
// 输出到控制台（如果启用）
#ifdef CONSOLE
QTextStream out(stdout);
out<< txt<< Qt::endl;
#endif
#define DEBUG true

void initTranslator(QApplication &a, const QString &path)
{
    // 初始化翻译管理器
    TranslationManager::setApplication(&a);

    // 设置自定义翻译路径
    TranslationManager::instance()->addTranslationPath(path);

    // 自动检测并设置系统语言
    QString sysLang = TranslationManager::instance()->detectSystemLanguage();
    TranslationManager::instance()->setLanguage("en_US");

    qDebug() << "System language: " << sysLang;
    qDebug() << "Translator initialized";
}

// 居然敢动我的屎山代码，想必你应该是做好了觉悟吧。

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    // 确保 必要 目录存在
    initFileSys();
    // 初始化日志
    initLogFile();
    // 设置控制台输出的代码页为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 初始化日志等级
    setLogLevel(read_log_level());

    // 安装自定义消息处理器
    if (!DEBUG) qInstallMessageHandler(messageHandler);
    //  初始化随机数生成器
    std::srand(std::time(nullptr));
    QApplication app(argc, argv);
    app.setApplicationName(DataManager::instance().Project_Name + " " + DataManager::instance().const_config_data.version);
    app.setFont(DataManager::instance()._font); //整个应用程序的界面都会使用这个字体
    app.setWindowIcon(QIcon(":/assets/image/Pelr.png"));
    QStringList styles = QStyleFactory::keys();

    qDebug() << "Available styles:" << styles;

    QString preferredStyle = "Fusion"; // Windows all platforms

    if (styles.contains(preferredStyle, Qt::CaseInsensitive))
    {
        app.setStyle(preferredStyle);
    }

    if (!CheckApplication::hasValidLicense()) {
        CheckApplication licenseDialog;
        if (!(licenseDialog.exec() == QDialog::Accepted && licenseDialog.isLicenseAccepted())) {
            qDebug() << "License not accepted, exit";
            return 0;
        }
        qDebug() << "License accepted";
    }

    GLCore w;

    TrayIcon::instance()->show();

    // 初始化 ONNX Runtime（onnxruntime.dll 已复制到 exe 目录，传空字符串自动查找）
    if (!VoicevoxTTS::initializeOnnxRuntime())
    {
        qWarning() << "Failed to initialize OnnxRuntime";
    }
    // 初始化翻译管理器
    initTranslator(app, ":/translations");

    // boot by silent mode
    if (DataManager::instance().getBasicData().isSilentBoot) {
        TrayIcon::instance()->action_silentMode->triggered();
    } else {
        w.show();
    }

    return app.exec();
}
