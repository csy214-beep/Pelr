/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class TrayIcon : public QSystemTrayIcon {
	Q_OBJECT

public:
	// 删除拷贝构造函数和赋值运算符
	TrayIcon(const TrayIcon &) = delete;

	TrayIcon &operator=(const TrayIcon &) = delete;

	// 获取单例实例
	static TrayIcon *instance();

	// 静态方法用于显示消息
	static void showMessage(const QString &title, const QString &msg,
	                        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information,
	                        int timeout = 10000);

	QAction *action_resetWinLoc;
	QAction *action_showWin;
	QAction *action_silentMode;
	QAction *action_switchDrag;
	QAction *action_switchTransparent;
	QAction *action_mediaPlayer;

private:
	TrayIcon(QObject *parent = nullptr);

	~TrayIcon();

	void switchText(QAction *action);

	// 单例实例指针
	static TrayIcon *m_instance;

	QMenu *menu;
	bool m_silentMode = false;
};

#endif // TRAY_H
