/*
PROJE  CT_NAME PLauncher
PRODUCT_NAME CLion
NAME CheckApplication
AUTHOR Pfolg
TIME 2025/10/14 21:04
*/
#include "CheckApplication.h"
#include "data.hpp"
#define LICENSE_CHECK_FILE "user/license.dat"

CheckApplication::CheckApplication(QWidget *parent)
    : QDialog(parent),
      m_licenseAccepted(false),
      m_licenseValidated(false) {
    // 获取系统信息
    m_username = getComputerName();
    m_version = getCurrentVersion();

    // 延迟许可证验证到 showEvent
    setupUI();
    setModal(true);
}

void CheckApplication::showEvent(QShowEvent *event) {
    // 在窗口显示前检查许可证
    if (!m_licenseValidated) {
        m_licenseValidated = true;

        if (validateExistingLicense()) {
            m_licenseAccepted = true;
            // 延迟调用 accept，确保对象完全构造
            QMetaObject::invokeMethod(this, "accept", Qt::QueuedConnection);
            return;
        }
    }

    QDialog::showEvent(event);
}

bool CheckApplication::hasValidLicense() {
    // 静态方法，用于在创建对话框前检查许可证
    QString username = QHostInfo::localHostName();
    QString version = DataManager::instance().const_config_data.version; // 与实际版本保持一致

    QFile file(LICENSE_CHECK_FILE);
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    QString savedUsername, savedVersion;
    QDateTime savedTimestamp;

    in >> savedUsername >> savedVersion >> savedTimestamp;
    file.close();

    // 验证用户名和版本是否匹配
    if (savedUsername == username && savedVersion == version) {
        // 检查许可证是否在有效期内
        if (savedTimestamp.daysTo(QDateTime::currentDateTime()) <= 365) {
            qDebug() << "找到有效的许可证文件";
            return true;
        }
    }

    return false;
}

void CheckApplication::setupUI() {
    setWindowTitle("MIT 许可证确认");
    setFixedSize(800, 600);

    // 创建控件
    QLabel *titleLabel = new QLabel("MIT 许可证协议", this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // MIT 许可证文本
    licenseText = new QTextEdit(this);
    licenseText->setReadOnly(true);

    QString mitLicense =
            "MIT 许可证\n\n"
            "版权所有 (c) " + QDateTime::currentDateTime().toString("yyyy") + " SY Cheng\n\n"
            "特此免费授予任何获得本软件及相关文档文件（以下简称\"软件\"）副本的人"
            "无限制地处理本软件的权限，包括但不限于使用、复制、修改、合并、发布、分发、再许可"
            "和/或销售本软件副本的权利，并允许向其提供本软件的人这样做，但须符合以下条件：\n\n"
            "上述版权声明和本许可声明应包含在本软件的所有副本或重要部分中。\n\n"
            "本软件按\"原样\"提供，不附带任何明示或暗示的保证，包括但不限于对适销性、"
            "特定用途的适用性和非侵权性的保证。在任何情况下，作者或版权持有人均不对"
            "因本软件或本软件的使用或其他交易而产生的任何索赔、损害赔偿或其他责任负责，"
            "无论是在合同诉讼、侵权行为还是其他方面。\n\n"
            "用户: " + m_username + "\n"
            "版本: " + m_version;

    licenseText->setPlainText(mitLicense);

    // 同意复选框
    agreeCheckbox = new QCheckBox("我理解并接受上述 MIT 许可证条款", this);

    // 按钮
    acceptButton = new QPushButton("接受", this);
    rejectButton = new QPushButton("拒绝", this);

    acceptButton->setEnabled(false);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(licenseText);
    mainLayout->addWidget(agreeCheckbox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(rejectButton);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(agreeCheckbox, &QCheckBox::stateChanged,
            [this](int state) { acceptButton->setEnabled(state == Qt::Checked); });
    connect(acceptButton, &QPushButton::clicked, this, &CheckApplication::onAcceptClicked);
    connect(rejectButton, &QPushButton::clicked, this, &CheckApplication::onRejectClicked);
}

bool CheckApplication::validateExistingLicense() {
    QFile file(LICENSE_CHECK_FILE);
    if (!file.exists()) {
        qDebug() << "许可证文件不存在";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开许可证文件进行读取";
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    QString savedUsername, savedVersion;
    QDateTime savedTimestamp;

    in >> savedUsername >> savedVersion >> savedTimestamp;
    file.close();

    if (savedUsername == m_username && savedVersion == m_version) {
        if (savedTimestamp.daysTo(QDateTime::currentDateTime()) <= 365) {
            qDebug() << "找到有效的许可证文件，用户:" << savedUsername << "版本:" << savedVersion;
            return true;
        } else {
            qDebug() << "许可证已过期";
        }
    }

    qDebug() << "许可证验证失败，期望用户:" << m_username << "实际:" << savedUsername
            << "期望版本:" << m_version << "实际:" << savedVersion;
    return false;
}

void CheckApplication::saveLicenseAgreement() {
    QDir dir;
    if (!dir.exists("user")) {
        dir.mkdir("user");
    }

    QFile file(LICENSE_CHECK_FILE);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_0);

        QDateTime timestamp = QDateTime::currentDateTime();
        out << m_username << m_version << timestamp;
        file.close();
        qDebug() << "许可证已保存，用户:" << m_username << "版本:" << m_version;
    } else {
        qDebug() << "无法保存许可证文件";
    }
}

QString CheckApplication::getComputerName() {
    return QHostInfo::localHostName();
}

QString CheckApplication::getCurrentVersion() {
    return DataManager::instance().const_config_data.version;
}

void CheckApplication::onAcceptClicked() {
    saveLicenseAgreement();
    m_licenseAccepted = true;
    accept();
}

void CheckApplication::onRejectClicked() {
    m_licenseAccepted = false;
    reject();
}
