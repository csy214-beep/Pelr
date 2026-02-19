/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include <QtWidgets/QApplication>
#include "GLCore.h"
#include "tray.h"
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <QString>
#include <QDir>
#include  "CheckApplication.h"
#include "TranslationManager.h"
#include "logger.hpp"
#include "NotificationWidget.h"
// 输出到控制台（如果启用）
#ifdef CONSOLE
QTextStream out(stdout);
out<< txt<< Qt::endl;
#endif


void initTranslator(QApplication &a, const QString &path) {
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

/*
*void LAppLive2DManager::OnTap(csmFloat32 x, csmFloat32 y)

CubismMotionQueueEntryHandle LAppModel::StartMotion(const csmChar* group, csmInt32 no, csmInt32 priority, ACubismMotion::FinishedMotionCallback onFinishedMotionHandler)

CubismMotionQueueEntryHandle LAppModel::StartRandomMotion(const csmChar* group, csmInt32 priority, ACubismMotion::FinishedMotionCallback onFinishedMotionHandler)
 */

/*
  居然敢动我的屎山代码，想必你应该是做好了觉悟吧。
*/
int main(int argc, char *argv[]) {
    // 初始化日志
    initLogFile();
    // 初始化日志等级
    setLogLevel(read_log_level());
    // 确保 user 目录存在
    QDir().mkpath("user"); // 如果目录不存在则创建，包括所有必要的父目录
    // 安装自定义消息处理器
    qInstallMessageHandler(messageHandler);
    //  初始化随机数生成器
    std::srand(std::time(nullptr));
    QApplication app(argc, argv);
    app.setFont(DataManager::instance()._font); //整个应用程序的界面都会使用这个字体
    app.setWindowIcon(QIcon(":/assets/image/Pelr.png"));

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
