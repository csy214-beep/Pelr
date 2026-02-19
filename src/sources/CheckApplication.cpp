/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
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
            qDebug() << "found valid license file";
            return true;
        }
    }

    return false;
}

void CheckApplication::setupUI() {
    setWindowTitle(tr("GNU GPL v3 许可证确认"));
    setFixedSize(800, 600);

    // 创建控件
    QLabel *titleLabel = new QLabel(tr("GNU 通用公共许可证 v3"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // MIT 许可证文本
    licenseText = new QTextEdit(this);
    licenseText->setReadOnly(true);

    QString LicenseText = tr(
            "GNU 通用公共许可证\n"
            "第三版，2007年6月29日\n\n"

            "版权所有 (C) 2007 自由软件基金会 <https://fsf.org/>\n\n"

            "本程序是自由软件：您可以根据自由软件基金会发布的 GNU 通用公共许可证"
            "的条款重新分发和/或修改它，可以是许可证的第3版，或者（根据您的选择）任何后续版本。\n\n"

            "分发本程序是希望它有用，但没有任何担保；甚至没有适销性或特定用途适用性的暗示担保。"
            "有关更多详细信息，请参阅 GNU 通用公共许可证。\n\n"

            "您应该已经随本程序收到了 GNU 通用公共许可证的副本。"
            "如果没有，请参阅 <https://www.gnu.org/licenses/>。\n\n"

            "本程序的主要条款：\n\n"
            "1. 您可以任何媒介运行本程序，无需付费\n"
            "2. 您可以学习程序如何工作，并对其进行修改\n"
            "3. 您可以重新分发副本\n"
            "4. 您可以发布修改后的版本\n\n"

            "当您重新分发本程序时，您必须：\n"
            "- 保留所有原始版权声明\n"
            "- 使用相同的 GPLv3 许可证\n"
            "- 提供源代码或获取源代码的方法\n"
            "- 保持所有通知完整无损\n\n"

            "详细信息请参阅完整的 GNU GPL v3 许可证文本。\n\n"
            "用户: %1\n"
            "版本: %2"
    ).arg(m_username).arg(m_version);
/*
            "GNU General Public License\n"
            "Version 3, 29 June 2007\n\n"

            "Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>\n\n"

            "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License "
            "as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n"

            "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of "
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\n"

            "You should have received a copy of the GNU General Public License along with this program. "
            "If not, see <https://www.gnu.org/licenses/>.\n\n"

            "Main terms of this program:\n\n"
            "1. You can run this program in any medium without paying a fee\n"
            "2. You can study how the program works and modify it\n"
            "3. You can redistribute copies of the program\n"
            "4. You can distribute modified versions of the program\n\n"

            "When you redistribute this program, you must:\n"
            "- Keep all original copyright notices intact\n"
            "- Use the same GPLv3 license\n"
            "- Provide the source code or a way to obtain the source code\n"
            "- Keep all notices complete and unaltered\n\n"

            "For details, see the full text of the GNU GPL v3 License.\n\n"
            "User: %1\n"
            "Version: %2"
*/
    licenseText->setPlainText(LicenseText);

    // 同意复选框
    agreeCheckbox = new QCheckBox(tr("我理解并接受上述 GNU GPL v3 许可证条款"), this);

    // 按钮
    acceptButton = new QPushButton(tr("接受"), this);
    rejectButton = new QPushButton(tr("拒绝"), this);

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
        qDebug() << "license check file not found";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "license check file open failed";
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
            qDebug() << "found valid license file, user:" << savedUsername << "version:" << savedVersion;
            return true;
        } else {
            qDebug() << "found expired license file";
        }
    }

    qDebug() << "license check failed, expected user:" << m_username << "actual user:" << savedUsername
             << "expected version:" << m_version << "actual version:" << savedVersion;
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
        qDebug() << "license agreement saved, user:" << m_username << "version:" << m_version;
    } else {
        qDebug() << "can't save license agreement";
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
