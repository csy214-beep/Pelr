/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef OLLAMACLIENT_H
#define OLLAMACLIENT_H

#include <QObject>
#include <QString>
#include <QMap>

class QNetworkAccessManager;
class QNetworkReply;

class OllamaClient : public QObject {
    Q_OBJECT

public:
    // 预定义角色类型
    enum Role {
        DefaultCoder, // 默认编程助手
        DesktopPetGirlfriend, // 桌宠女友
        TechnicalTeacher, // 技术导师
        CreativeWriter, // 创意写作助手
        CustomRole // 自定义角色
    };

    Q_ENUM(Role)

    struct RoleItem {
        QString text;
        Role role;
    };

    static const QList<RoleItem> Roles;

    explicit OllamaClient(QObject *parent = nullptr);

    ~OllamaClient();

    // 同步生成文本（阻塞调用）
    QString generateText(const QString &prompt,
                         const QString &model = "llama3",
                         bool stream = false,
                         Role role = DefaultCoder,
                         const QString &customRoleName = "");

    // 异步生成文本（非阻塞，通过信号返回结果）
    void generateTextAsync(const QString &prompt,
                           const QString &model = "llama3",
                           bool stream = false,
                           Role role = DefaultCoder,
                           const QString &customRoleName = "");

    // 设置自定义角色提示词
    void setCustomRolePrompt(const QString &roleName, const QString &prompt);

    // 获取角色提示词
    QString getRolePrompt(Role role, const QString &customRoleName = "") const;

signals:
    // 异步请求完成信号
    void textGenerated(const QString &text);

    // 错误信号
    void errorOccurred(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QMap<QString, QString> m_customRoles; // 存储自定义角色和对应的提示词

    // 根据角色类型获取提示词
    QString getPromptByRole(Role role, const QString &customRoleName = "") const;

    // 构建完整的提示词（角色描述 + 用户输入）
    QString buildFullPrompt(const QString &rolePrompt, const QString &userPrompt) const;
};

#endif // OLLAMACLIENT_H
