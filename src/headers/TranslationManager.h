/*
 * PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <QMap>
#include <QDir>

class TranslationManager : public QObject {
Q_OBJECT

public:
    // 获取单例实例
    static TranslationManager *instance();

    // 设置应用程序对象（必须在主函数中调用）
    static void setApplication(QApplication *app);

    // 加载翻译文件
    bool loadTranslation(const QString &languageCode);

    // 获取支持的语言列表
    QStringList availableLanguages() const;

    // 获取当前语言
    QString currentLanguage() const;

    // 设置语言
    void setLanguage(const QString &languageCode);

    // 自动检测系统语言
    QString detectSystemLanguage();

    // 添加翻译文件路径
    void addTranslationPath(const QString &path);

signals:

    // 语言改变信号
    void languageChanged(const QString &languageCode);

private:
    // 私有构造函数
    explicit TranslationManager(QObject *parent = nullptr);

    // 禁止拷贝
    TranslationManager(const TranslationManager &) = delete;

    TranslationManager &operator=(const TranslationManager &) = delete;

    // 单例实例
    static TranslationManager *m_instance;
    static QApplication *m_app;

    // 当前翻译器
    QTranslator *m_translator;

    // 翻译文件路径列表
    QStringList m_translationPaths;

    // 语言映射：代码->显示名称
    QMap<QString, QString> m_languages;

    // 当前语言代码
    QString m_currentLanguage;

    // 初始化支持的语言
    void initLanguages();

    // 查找翻译文件
    QString findTranslationFile(const QString &languageCode);
};

#endif // TRANSLATIONMANAGER_H