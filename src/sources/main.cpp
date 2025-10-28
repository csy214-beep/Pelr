/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
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
#define QT_LOG_FILE "log/plauncher_qt.log"
// 输出到控制台（如果启用）
#ifdef CONSOLE
QTextStream out(stdout);
out<< txt<< Qt::endl;
#endif

// 自定义消息处理函数
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context)

    QString logTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString txt;
    switch (type) {
        case QtDebugMsg:
            txt = QString("[%1] Debug: %2").arg(logTime).arg(msg);
            break;
        case QtInfoMsg:
            txt = QString("[%1] Info: %2").arg(logTime).arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("[%1] Warning: %2").arg(logTime).arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("[%1] Critical: %2").arg(logTime).arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("[%1] Fatal: %2").arg(logTime).arg(msg);
            break;
    }

    // 输出到日志文件
    QFile logFile(QT_LOG_FILE);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream textStream(&logFile);
        textStream.setCodec("UTF-8");
        textStream << txt << Qt::endl;
        logFile.close();
    }
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
    // 确保 user 目录存在
    QDir().mkpath("user"); // 如果目录不存在则创建，包括所有必要的父目录
    QDir().mkpath("log"); // 如果目录不存在则创建，包括所有必要的父目录
    QFile::remove(QT_LOG_FILE); // 删除日志文件
    // 安装自定义消息处理器
    // qInstallMessageHandler(messageHandler);
    //  初始化随机数生成器
    std::srand(std::time(nullptr));
    QApplication app(argc, argv);
    app.setFont(DataManager::instance()._font); //整个应用程序的界面都会使用这个字体
    app.setWindowIcon(QIcon(":/assets/image/PLauncher.png"));

    if (!CheckApplication::hasValidLicense()) {
        CheckApplication licenseDialog;
        if (!(licenseDialog.exec() == QDialog::Accepted && licenseDialog.isLicenseAccepted())) {
            qDebug() << "应用程序因许可证拒绝而退出";
            return 0;
        }
        qDebug() << "许可证已被接受";
    }

    GLCore w;

    //重置window位置
    QObject::connect(TrayIcon::instance()->action_resetWinLoc, SIGNAL(triggered()), &w, SLOT(resetLocation()));
    //显示界面
    QObject::connect(TrayIcon::instance()->action_showWin,SIGNAL(triggered()), w.main_widget,SLOT(show()));
    //静默模式
    QObject::connect(TrayIcon::instance()->action_silentMode,SIGNAL(triggered()), &w,SLOT(silentMode()));
    //拖动窗口
    QObject::connect(TrayIcon::instance()->action_switchDrag,SIGNAL(triggered()), &w,SLOT(switchDragStatus()));
    //播放媒体
    QObject::connect(TrayIcon::instance()->action_mediaPlayer,SIGNAL(triggered()), &w,SLOT(onPlayMedia()));

    TrayIcon::instance()->show();
    w.show();

    return app.exec();
}
