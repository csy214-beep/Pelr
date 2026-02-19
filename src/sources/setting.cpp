/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "setting.h"
#include <QFileDialog>
#include  <QString>
#include "launcher.hpp"
#include <windows.h>
#include <shlobj.h>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QColorDialog>
#include <QColor>
#include <QDebug>
#include "LAppLive2DManager.hpp"
#include  "logger.hpp"
#include "ollamaclient.h"
#include "tray.h"
#include "NotificationWidget.h"

ConfigData SettingWidget::getAllValues() {
    ConfigData data;
    //basic
    data.model_path = ui->lineEdit->text();
    QFileInfo fi(data.model_path);
    // 检查文件是否存在
    if (fi.exists() && fi.isFile()) {
        qDebug() << "File exists: " << data.model_path;
    } else {
        qWarning() << "File not exists: " << data.model_path;
        data.model_path = "";
    }
    data.model_size = ui->horizontalSlider->value();
    data.FPS = ui->horizontalSlider_2->value();
    data.volume = ui->horizontalSlider_3->value();
    data.RandomInterval.first = ui->spinBox->value();
    data.RandomInterval.second = ui->spinBox_2->value();
    // basic color
    data.color_bubble.first = ui->lineEdit_9->text();
    data.color_bubble.second = ui->lineEdit_10->text();
    data.color_keyLabel.first = ui->lineEdit_11->text();
    data.color_keyLabel.second = ui->lineEdit_12->text();
    //bool
    data.isStartUp = ui->checkBox->isChecked();
    data.isListening = ui->checkBox_2->isChecked();
    data.isLookingMouse = ui->checkBox_3->isChecked();
    data.isStartStar = ui->checkBox_4->isChecked();
    data.isRandomSpeech = ui->checkBox_5->isChecked();
    data.isSaying = ui->checkBox_6->isChecked();
    data.isHourAlarm = ui->checkBox_7->isChecked();
    data.isTop = ui->checkBox_8->isChecked();
    data.isTrayHourAlarm = ui->checkBox_9->isChecked();
    data.isSilentBoot = ui->checkBox_10->isChecked();
    data.isRecordWindowLocation = ui->checkBox_11->isChecked();
    data.isMusicIcon = ui->checkBox_12->isChecked();

    //Ollama
    data.model = ui->lineEdit_6->text();
    data.role = static_cast<OllamaClient::Role>(ui->comboBox->currentIndex());
    data.customRoleDesc = ui->plainTextEdit->toPlainText();

    return data;
}

TTSConfig SettingWidget::getTTSConfigValue() const {
    TTSConfig data;
    //TTS
    data.APPID = ui->lineEdit_2->text();
    data.APISecret = ui->lineEdit_3->text();
    data.APIKey = ui->lineEdit_4->text();
    data.speaker = ui->lineEdit_5->text();
    return data;
}

OpenWeatherData SettingWidget::getOpenWeatherDataValue() {
    OpenWeatherData data;
    //OpenWeather
    data.city = ui->lineEdit_7->text();
    data.api_key = ui->lineEdit_8->text();
    return data;
}

void SettingWidget::setTTSConfig(const TTSConfig &data) const {
    //TTS
    ui->lineEdit_2->setText(data.APIKey);
    ui->lineEdit_3->setText(data.APISecret);
    ui->lineEdit_4->setText(data.APPID);
    ui->lineEdit_5->setText(data.speaker);
}

void SettingWidget::setOpenWeatherData(const OpenWeatherData &data) const {
    //OpenWeather
    ui->lineEdit_7->setText(data.city);
    ui->lineEdit_8->setText(data.api_key);
}

void SettingWidget::setAllValues(const ConfigData &data) {
    ui->label_2->setText(DataManager::instance().const_config_data.version);
    //basic
    ui->lineEdit->setText(data.model_path);
    ui->horizontalSlider->setValue(data.model_size);
    ui->horizontalSlider_2->setValue(data.FPS);
    ui->horizontalSlider_3->setValue(data.volume);
    ui->label_7->setText(QString::number(data.model_size));
    ui->label_8->setText(QString::number(data.FPS));
    ui->label_9->setText(QString::number(data.volume));
    ui->spinBox->setValue(data.RandomInterval.first);
    ui->spinBox_2->setValue(data.RandomInterval.second);
    ui->comboBox_3->setCurrentIndex(static_cast<int>(getLogLevel()));
    // color
    ui->lineEdit_9->setText(data.color_bubble.first);
    ui->label_24->setStyleSheet("background-color: " + data.color_bubble.first + ";");
    ui->lineEdit_10->setText(data.color_bubble.second);
    ui->label_25->setStyleSheet("color: " + data.color_bubble.second + ";");
    ui->lineEdit_11->setText(data.color_keyLabel.first);
    ui->label_26->setStyleSheet("background-color: " + data.color_keyLabel.first + ";");
    ui->lineEdit_12->setText(data.color_keyLabel.second);
    ui->label_27->setStyleSheet("color: " + data.color_keyLabel.second + ";");
    //bool
    ui->checkBox->setChecked(data.isStartUp);
    if (checkStartupLink()) {
        ui->checkBox->setChecked(true);
    } else {
        ui->checkBox->setChecked(false);
    }
    ui->checkBox_2->setChecked(data.isListening);
    ui->checkBox_3->setChecked(data.isLookingMouse);
    ui->checkBox_4->setChecked(data.isStartStar);
    ui->checkBox_5->setChecked(data.isRandomSpeech);
    ui->checkBox_6->setChecked(data.isSaying);
    ui->checkBox_7->setChecked(data.isHourAlarm);
    ui->checkBox_8->setChecked(data.isTop);
    ui->checkBox_9->setChecked(data.isTrayHourAlarm);
    ui->checkBox_10->setChecked(data.isSilentBoot);
    ui->checkBox_11->setChecked(data.isRecordWindowLocation);
    ui->checkBox_12->setChecked(data.isMusicIcon);

    //Ollama
    ui->lineEdit_6->setText(data.model);
    ui->comboBox->setCurrentIndex(static_cast<int>(data.role));
    ui->plainTextEdit->setPlainText(data.customRoleDesc);
}

SettingWidget::SettingWidget(QWidget *parent) : QWidget(parent), ui(new Ui::setting) {
    ui->setupUi(this);
    // 检查更新
    m_versionChecker = new VersionChecker(this);
    //basic
    ui->horizontalSlider->setRange(80, 600); //size
    ui->horizontalSlider_2->setRange(15, 120); //FPS
    ui->horizontalSlider_3->setRange(0, 100); //volume
    //保密
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    ui->lineEdit_8->setEchoMode(QLineEdit::Password);
    //Ollama
    ui->comboBox->clear();
    for (const auto &item: OllamaClient::Roles) {
        ui->comboBox->addItem(item.text, static_cast<int>(item.role));
    }
    //说明
    QFile file(":/assets/text/thirdParty.md");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为 UTF-8
        QString content = in.readAll();
        file.close();
        ui->textEdit->setMarkdown(
            "### Pelr " + DataManager::instance().const_config_data.version + "\n\n" + content);
    }
    ui->textEdit->setReadOnly(true);
    // 初始化日志等级选择
    struct LogLevelItem {
        QString text;
        LogLevel value;
    };

    QVector<LogLevelItem> logLevels = {
        {tr("调试信息"), LogLevel::Debug},
        {tr("普通信息"), LogLevel::Info},
        {tr("警告"), LogLevel::Warning},
        {tr("严重错误"), LogLevel::Critical},
        {tr("致命错误"), LogLevel::Fatal}
    };

    ui->comboBox_3->clear();
    for (const auto &item: logLevels) {
        ui->comboBox_3->addItem(item.text, static_cast<int>(item.value));
    }

    // 连接信号槽
    connectSignals();
    // 设置初始值
    setAllValues(DataManager::instance().getBasicData());
    setTTSConfig(DataManager::instance().getTTSConfig());
    setOpenWeatherData(DataManager::instance().getOpenWeatherData());
}

void SettingWidget::connectSignals() {
    //basic
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SettingWidget::selectModelPath);
    connect(ui->horizontalSlider, &QSlider::valueChanged, [&]() {
        ui->label_7->setText(QString::number(ui->horizontalSlider->value()));
    });
    connect(ui->label_7, &DoubleClickableLabel::doubleClicked, [&]() {
        ui->horizontalSlider->setValue(150);
    });
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, [&]() {
        ui->label_8->setText(QString::number(ui->horizontalSlider_2->value()));
    });
    connect(ui->label_8, &DoubleClickableLabel::doubleClicked, [&]() {
        ui->horizontalSlider_2->setValue(30);
    });
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, [&]() {
        ui->label_9->setText(QString::number(ui->horizontalSlider_3->value()));
    });
    connect(ui->label_9, &DoubleClickableLabel::doubleClicked, [&]() {
        ui->horizontalSlider_3->setValue(50);
    });
    connect(ui->comboBox_3, &QComboBox::currentTextChanged, this, &SettingWidget::onLogLevelChanged);
    //color
    connect(ui->lineEdit_9, &QLineEdit::returnPressed, [&]() {
        selectColor(ui->label_24, ui->lineEdit_9);
    });
    connect(ui->lineEdit_10, &QLineEdit::returnPressed, [&]() {
        selectColor(ui->label_25, ui->lineEdit_10, false);
    });
    connect(ui->lineEdit_11, &QLineEdit::returnPressed, [&]() {
        selectColor(ui->label_26, ui->lineEdit_11);
    });
    connect(ui->lineEdit_12, &QLineEdit::returnPressed, [&]() {
        selectColor(ui->label_27, ui->lineEdit_12, false);
    });
    //bool
    connect(ui->checkBox, &QCheckBox::clicked, [&]() {
        onCheckBox1Clicked(!ui->checkBox->isChecked());
    });
    connect(ui->checkBox_12, &QCheckBox::clicked, [&]() {
        TrayIcon::instance()->switchMusicIcon(ui->checkBox_12->isChecked());
    });

    // 链接跳转
    connect(ui->pushButton_3, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.tts_url);
    });
    connect(ui->pushButton_14, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.tts_server);
    });
    connect(ui->pushButton_15, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.ollama_url);
    });
    connect(ui->pushButton_4, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.openWeather_url);
    });
    //github
    connect(ui->pushButton_8, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.website_link);
    });
    //author
    connect(ui->pushButton_9, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.team_link);
    });
    //issues
    connect(ui->pushButton_6, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.feedback_link);
    });
    //text
    connect(ui->pushButton_12, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.textFile);
    });
    //.
    connect(ui->pushButton_13, &QPushButton::clicked, [&]() {
        launchByPath(".");
    });
    //voiceFolder
    connect(ui->pushButton_5, &QPushButton::clicked, [&]() {
        launchByPath(DataManager::instance().const_config_data.VoiceFolder);
    });
    //reset
    connect(ui->pushButton_11, &QPushButton::clicked, this, &SettingWidget::resetSetting);
    //save
    connect(ui->pushButton, &QPushButton::clicked, [&]() {
        saveData();
        NotificationWidget::showNotification(tr("Information"), tr("保存成功！\n下次应用启动生效。"));
    });
    // 检查更新
    connect(ui->pushButton_10, &QPushButton::clicked, [&]() {
        m_versionChecker->checkVersionMatch(DataManager::instance().const_config_data.version);
    });
    connect(m_versionChecker, &VersionChecker::versionCheckCompleted,
            this, &SettingWidget::onVersionCheckCompleted);
    connect(m_versionChecker, &VersionChecker::errorOccurred,
            this, &SettingWidget::onVersionCheckError);
}

QSlider *SettingWidget::getHorizontalSlider() {
    return ui->horizontalSlider;
}

void SettingWidget::saveData() {
    ConfigData data = getAllValues();
    TTSConfig tts_data = getTTSConfigValue();
    OpenWeatherData weather_data = getOpenWeatherDataValue();
    // 保存配置
    DataManager::instance().writeData<ConfigData>(data);
    DataManager::writeTTSConfig(tts_data);
    DataManager::writeOpenWeatherData(weather_data);
    qDebug() << "save data success";
}

bool SettingWidget::checkStartupLink() {
    // 获取启动文件夹路径
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString appData = env.value("APPDATA");

    if (appData.isEmpty()) {
        appData = env.value("USERPROFILE");
        if (appData.isEmpty()) {
            qWarning() << "cannot get startup folder path";
            return false;
        }
        appData.append("/AppData/Roaming");
    }

    QString startupFolder = appData + "/Microsoft/Windows/Start Menu/Programs/Startup";

    // 获取当前应用程序路径
    QString executablePath = QCoreApplication::applicationFilePath();
    if (executablePath.isEmpty()) {
        qWarning() << "cannot get application path";
        return false;
    }

    // 构建快捷方式名称和路径
    QString shortcutName = QFileInfo(executablePath).baseName() + ".lnk";
    QString shortcutPath = QDir(startupFolder).filePath(shortcutName);
    if (!QFile::exists(shortcutPath)) {
        qDebug() << "shortcut not exists: " << shortcutPath;
        return false;
    }
    return true;
}

void SettingWidget::resetSetting() {
    const auto re = QMessageBox::question(
        this, tr("Confirmation"), tr("确定要重置设置吗？\n重置后将恢复默认设置！"),
        QMessageBox::Yes | QMessageBox::No);
    if (re == QMessageBox::No)return;
    const ConfigData new_data = ConfigData();
    setAllValues(new_data);
    DataManager::instance().writeData<ConfigData>(new_data);
    // 重置启动项（移除）
    onCheckBox1Clicked(true);
    qDebug() << "reset setting success";
    NotificationWidget::showNotification(tr("Information"), tr("重置设置成功！"));
}

void SettingWidget::onCheckBox1Clicked(const bool flag) {
    // 获取启动文件夹路径
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString appData = env.value("APPDATA");

    if (appData.isEmpty()) {
        appData = env.value("USERPROFILE");
        if (appData.isEmpty()) {
            qWarning() << "cannot get startup folder path";
        }
        appData.append("/AppData/Roaming");
    }

    QString startupFolder = appData + "/Microsoft/Windows/Start Menu/Programs/Startup";

    // 获取当前应用程序路径
    QString executablePath = QCoreApplication::applicationFilePath();
    if (executablePath.isEmpty()) {
        qWarning() << "cannot get application path";
    }

    // 构建快捷方式名称和路径
    QString shortcutName = QFileInfo(executablePath).baseName() + ".lnk";
    QString shortcutPath = QDir(startupFolder).filePath(shortcutName);
    if (!ui->checkBox->isChecked() || flag) {
        // 移除启动项
        if (!QFile::exists(shortcutPath)) {
            qDebug() << "shortcut not exists: " << shortcutPath;
            NotificationWidget::showNotification(tr("Information"), tr("快捷方式已不存在！"));
            return;
        }
        bool success = QFile::remove(shortcutPath);
        if (!success) {
            qWarning() << "cannot remove shortcut: " << shortcutPath;
            NotificationWidget::showNotification(tr("Warning"), tr("移除快捷方式失败！"));
            return;
        }
        qDebug() << "remove shortcut success: " << shortcutPath;
        NotificationWidget::showNotification(tr("Information"), tr("快捷方式已移除！"));
    } else {
        // 添加启动项
        IShellLink *pShellLink = nullptr;
        IPersistFile *pPersistFile = nullptr;

        HRESULT hr = CoInitialize(nullptr);
        if (SUCCEEDED(hr)) {
            hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink,
                                  (void **) &pShellLink);
            if (SUCCEEDED(hr)) {
                pShellLink->SetPath(executablePath.toStdWString().c_str());
                pShellLink->SetWorkingDirectory(QFileInfo(executablePath).absolutePath().toStdWString().c_str());
                hr = pShellLink->QueryInterface(IID_IPersistFile, (void **) &pPersistFile);
                if (SUCCEEDED(hr)) {
                    hr = pPersistFile->Save(shortcutPath.toStdWString().c_str(), TRUE);
                    pPersistFile->Release();
                }
                pShellLink->Release();
            }
            CoUninitialize();
        }

        if (FAILED(hr)) {
            qWarning() << "cannot create shortcut: " << shortcutPath;
            NotificationWidget::showNotification(tr("Warning"), tr("创建快捷方式失败！"));
        } else {
            qDebug() << "create shortcut success: " << shortcutPath;
            NotificationWidget::showNotification(tr("Information"), tr("快捷方式已创建！"));
        }
    }
}

void SettingWidget::selectColor(QLabel *label, QLineEdit *line, bool isBackground) {
    // 创建颜色选择对话框
    QColorDialog dialog;
    dialog.setOption(QColorDialog::ShowAlphaChannel, true); // 启用透明度选项
    dialog.setCurrentColor(QColor(255, 0, 0, 128)); // 设置初始颜色（半透明红色）

    // 显示对话框并获取结果
    if (dialog.exec() == QDialog::Accepted) {
        QColor selectedColor = dialog.currentColor();
        QString colorStr = selectedColor.name(QColor::HexArgb);
        qDebug() << "chose color: " << colorStr;
        if (isBackground) {
            label->setStyleSheet("background-color: " + colorStr + ";");
        } else {
            label->setStyleSheet("color: " + colorStr + ";");
        }
        line->setText(colorStr);
    }
}

void SettingWidget::onVersionCheckCompleted(bool isMatch, const QString &message) {
    NotificationWidget::MessageType type;
    if (isMatch) {
        // 版本一致的处理
        type = NotificationWidget::MessageType::Information;
    } else {
        // 版本不一致的处理
        type = NotificationWidget::MessageType::Warning;
    }
    NotificationWidget::showNotification(tr("Information"), message, 5000, type);
}

void SettingWidget::onVersionCheckError(const QString &errorMessage) {
    NotificationWidget::showNotification(tr("Warning"), errorMessage, 5000, NotificationWidget::MessageType::Critical);
}

void SettingWidget::onLogLevelChanged() {
    LogLevel level = static_cast<LogLevel>(ui->comboBox_3->currentData().toInt());
    write_log_level(level);
    qDebug() << "set logLevel to" << static_cast<int>(level);
}

void SettingWidget::selectModelPath() {
    QString folder = "./Resources/";
    QDir dir(folder);
    //确保文件夹存在
    if (!dir.exists())QDir().mkpath(folder);
    QString path = QFileDialog::getOpenFileName(this, tr("选择模型文件"), folder, "*.model3.json");
    if (!path.isEmpty()) {
        ui->lineEdit->setText(path);
        QFileInfo file_info(path);
        QString model_dir = file_info.absolutePath();
        QString fileName = file_info.fileName();
        qDebug() << "model dir: " << model_dir << " fileName: " << fileName;
        LAppLive2DManager::GetInstance()->LoadModelFromPath(model_dir.toStdString() + "/", fileName.toStdString());
        // 保存配置 用以加载模型表情
        saveData();
    }
}

SettingWidget::~SettingWidget() {
    delete ui;
}
