/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://github.com/csy214-beep/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "setting.h"
#include <QFileDialog>
#include <QString>
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
#include "logger.hpp"
#include "llamaclient.h"
#include "tray.h"
#include "NotificationWidget.h"
#include "loadText.h"
#include "UpdateDialog.h"
#include "voicevox_tts.h"
#include "voicegenerator.hpp"
using MessageType = NotificationWidget::MessageType;

ConfigData SettingWidget::getAllValues()
{
    ConfigData data;
    // basic
    data.model_path = ui->lineEdit->text();
    QFileInfo fi(data.model_path);
    // 检查文件是否存在
    if (fi.exists() && fi.isFile())
    {
        qDebug() << "File exists: " << data.model_path;
    }
    else
    {
        qWarning() << "File not exists: " << data.model_path;
        data.model_path = "";
    }
    data.model_size = ui->horizontalSlider->value();
    data.FPS = ui->horizontalSlider_2->value();
    data.volume = ui->horizontalSlider_3->value();
    data.RandomInterval.first = ui->spinBox->value();
    data.RandomInterval.second = ui->spinBox_2->value();
    // basic color
    data.color_bubble.background = ui->label_24->text(); // ARGB
    data.color_bubble.forground = ui->label_25->text();
    // tray
    data.color_tray.forground = ui->label_34->text();
    data.color_tray.background = ui->label_35->text();
    data.music_tray_symbol = ui->lineEdit_9->text().isEmpty() ? "♫" : ui->lineEdit_9->text();

    // bool
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
    data.isShowThinkingBubble = ui->checkBox_14->isChecked();

    return data;
}

TTSConfig SettingWidget::getTTSConfigValue() const
{
    TTSConfig data;
    data.provider = ui->comboBox_4->currentIndex();
    qDebug() << "TTS provider: " << data.provider;
    data.tr_point = ui->comboBox_7->currentIndex();
    qDebug() << "TRA provider: " << data.tr_point;
    // openai_edge_tts
    data.speaker_openai_edge_tts = ui->lineEdit_11->text();
    data.speed_openai_edge_tts = ui->doubleSpinBox->value();
    // iFlytek
    data.iFlytek_APPID = ui->lineEdit_2->text();
    data.iFlytek_APISecret = ui->lineEdit_3->text();
    data.iFlytek_APIKey = ui->lineEdit_4->text();
    data.iFlytek_speaker = ui->lineEdit_5->text();
    // voicevox
    data.voicevox_dict_dir = ui->lineEdit_15->text();
    data.voicevox_model = ui->lineEdit_16->text();
    data.voicevox_style_id = ui->comboBox_6->currentData().toInt(); // 固定使用 userData 中的 ID
    data.voicevox_speed = ui->doubleSpinBox_2->value();
    // translate
    data.isRunTTSServerOnStartUp = ui->checkBox_13->isChecked();
    data.tr_lang_translators = ui->comboBox_5->currentText();
    data.tr_provider = ui->comboBox->currentText();
    data.tr_libretranslate_port = ui->lineEdit_17->text();
    data.tr_lang_libretranslate = ui->lineEdit_18->text();

    return data;
}

OpenWeatherData SettingWidget::getOpenWeatherDataValue()
{
    OpenWeatherData data;
    // OpenWeather
    data.city = ui->lineEdit_7->text();
    data.api_key = ui->lineEdit_8->text();
    return data;
}
LlamaData SettingWidget::getLlamaDataValue() const
{
    LlamaData llm;
    llm.baseUrl = ui->lineEdit_10->text();
    llm.model = ui->lineEdit_6->text();
    llm.apiKey = ui->lineEdit_12->text();
    llm.maxContextMessages = ui->spinBox_3->value();
    llm.promptFilePath = ui->lineEdit_13->text();
    llm.systemPrompt = ui->plainTextEdit->toPlainText();

    LlamaClient::instance()->configure(llm.baseUrl, llm.apiKey, llm.model, llm.systemPrompt, llm.maxContextMessages);
    LlamaClient::instance()->loadSystemPromptFromFile(llm.promptFilePath);

    return llm;
}

void SettingWidget::setTTSConfig(const TTSConfig &data) const
{
    // TTS
    qDebug() << " TTS provider " << data.provider;
    ui->comboBox_4->setCurrentIndex(static_cast<int>(data.provider));
    qDebug() << " TRA point " << data.tr_point;
    ui->comboBox_7->setCurrentIndex(static_cast<int>(data.tr_point));
    // openai-edge-tts
    ui->lineEdit_11->setText(data.speaker_openai_edge_tts);
    ui->doubleSpinBox->setValue(data.speed_openai_edge_tts);
    // iFlytek
    ui->lineEdit_2->setText(data.iFlytek_APIKey);
    ui->lineEdit_3->setText(data.iFlytek_APISecret);
    ui->lineEdit_4->setText(data.iFlytek_APPID);
    ui->lineEdit_5->setText(data.iFlytek_speaker);
    ui->checkBox_13->setChecked(data.isRunTTSServerOnStartUp);
    // voicevox
    ui->lineEdit_15->setText(data.voicevox_dict_dir);
    ui->lineEdit_16->setText(data.voicevox_model);
    ui->doubleSpinBox_2->setValue(data.voicevox_speed); // 改用 doubleSpinBox_2

    // 根据 style ID 选中风格
    int idx = ui->comboBox_6->findData(data.voicevox_style_id);
    if (idx >= 0)
    {
        ui->comboBox_6->setCurrentIndex(idx);
    }
    else
    {
        // 如果 ID 不存在（例如模型未加载），保留空白或第一个
        if (ui->comboBox_6->count() > 0)
            ui->comboBox_6->setCurrentIndex(0);
    }
    // translate
    ui->comboBox_5->setCurrentText(data.tr_lang_translators);
    ui->comboBox->setCurrentText(data.tr_provider);
    ui->lineEdit_17->setText(data.tr_libretranslate_port);
    ui->lineEdit_18->setText(data.tr_lang_libretranslate);
}
void SettingWidget::setLlamaData(const LlamaData &llm) const
{
    ui->lineEdit_10->setText(llm.baseUrl);
    ui->lineEdit_6->setText(llm.model);
    ui->lineEdit_12->setText(llm.apiKey);
    ui->spinBox_3->setValue(llm.maxContextMessages);
    ui->lineEdit_13->setText(llm.promptFilePath);
    ui->plainTextEdit->setPlainText(llm.systemPrompt);
    LlamaClient::instance()->configure(
        llm.baseUrl,
        llm.apiKey,
        llm.model,
        llm.systemPrompt,
        llm.maxContextMessages);
    LlamaClient::instance()->loadSystemPromptFromFile(llm.promptFilePath);
}

void SettingWidget::setOpenWeatherData(const OpenWeatherData &data) const
{
    // OpenWeather
    ui->lineEdit_7->setText(data.city);
    ui->lineEdit_8->setText(data.api_key);
}

void SettingWidget::setAllValues(const ConfigData &data)
{
    ui->label_2->setText(DataManager::instance().const_config_data.version);
    // basic
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
    ui->label_24->setStyleSheet(
        QString("background-color: %1;color: %2;").arg(data.color_bubble.background).arg(data.color_bubble.forground));
    ui->label_24->setText(data.color_bubble.background);
    ui->label_25->setStyleSheet(
        QString("background-color: %1;color: %2;").arg(data.color_bubble.background).arg(data.color_bubble.forground));
    ui->label_25->setText(data.color_bubble.forground);
    // tray
    ui->label_34->setStyleSheet(QString("color: %1;").arg(data.color_tray.forground));
    ui->label_35->setStyleSheet(QString("color: %1;").arg(data.color_tray.background));
    ui->label_34->setText(data.color_tray.forground);
    ui->label_35->setText(data.color_tray.background);
    ui->lineEdit_9->setText(data.music_tray_symbol);
    // bool
    ui->checkBox->setChecked(data.isStartUp);
    if (checkStartupLink())
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
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
    ui->checkBox_14->setChecked(data.isShowThinkingBubble);
}

SettingWidget::SettingWidget(QWidget *parent) : QWidget(parent), ui(new Ui::setting)
{
    ui->setupUi(this);
    // 检查更新
    m_versionChecker = new VersionChecker(this);
    // 语言获取
    m_langClient = new PyLang(this);
    // basic
    ui->horizontalSlider->setRange(80, 600);   // size
    ui->horizontalSlider_2->setRange(15, 120); // FPS
    ui->horizontalSlider_3->setRange(0, 100);  // volume

    // 说明
    QFile file(":/CREDITS.md");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为 UTF-8
        QString content = in.readAll();
        file.close();
        ui->textBrowser->setMarkdown(content);
    }
    ui->textBrowser->setReadOnly(true);
    ui->textBrowser->setOpenExternalLinks(true);
    // 初始化日志等级选择
    struct LogLevelItem
    {
        QString text;
        LogLevel value;
    };

    QVector<LogLevelItem> logLevels = {
        {tr("调试信息"), LogLevel::Debug},
        {tr("普通信息"), LogLevel::Info},
        {tr("警告"), LogLevel::Warning},
        {tr("严重错误"), LogLevel::Critical},
        {tr("致命错误"), LogLevel::Fatal}};

    ui->comboBox_3->clear();
    for (const auto &item : logLevels)
    {
        ui->comboBox_3->addItem(item.text, static_cast<int>(item.value));
    }
    ui->comboBox_4->clear();
    for (const auto &item : TTSProviderList)
    {
        ui->comboBox_4->addItem(item.first, item.second);
    }
    ui->comboBox_7->clear();
    for (const auto &item : Translators)
    {
        ui->comboBox_7->addItem(item.first, item.second);
    }

    // 连接信号槽
    connectSignals();
    // 设置初始值
    setAllValues(DataManager::instance().getBasicData());
    setTTSConfig(DataManager::instance().getTTSConfig());
    setOpenWeatherData(DataManager::instance().getOpenWeatherData());
    setLlamaData(DataManager::instance().getLlamaData());
}

void SettingWidget::connectSignals()
{
    // basic
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SettingWidget::selectModelPath);
    connect(ui->horizontalSlider, &QSlider::valueChanged, [&]()
            { ui->label_7->setText(QString::number(ui->horizontalSlider->value())); });
    connect(ui->label_7, &DoubleClickableLabel::doubleClicked, [&]()
            { ui->horizontalSlider->setValue(150); });
    connect(ui->horizontalSlider_2, &QSlider::valueChanged, [&]()
            { ui->label_8->setText(QString::number(ui->horizontalSlider_2->value())); });
    connect(ui->label_8, &DoubleClickableLabel::doubleClicked, [&]()
            { ui->horizontalSlider_2->setValue(30); });
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, [&]()
            { ui->label_9->setText(QString::number(ui->horizontalSlider_3->value())); });
    connect(ui->label_9, &DoubleClickableLabel::doubleClicked, [&]()
            { ui->horizontalSlider_3->setValue(50); });
    connect(ui->comboBox_3, &QComboBox::currentTextChanged, this, &SettingWidget::onLogLevelChanged);
    // color
    connect(ui->pushButton_17, &QPushButton::clicked, [&]()
            {
        //bg
        QString color = selectColor();
        if (color.isEmpty())return;
        ui->label_24->setText(color);
        ui->label_24->setStyleSheet(
            QString("background-color: %1;color: %2;").arg(color).arg(ui->label_25->text()));
        ui->label_25->setStyleSheet(
            QString("background-color: %1;color: %2;").arg(color).arg(ui->label_25->text())); });
    connect(ui->pushButton_18, &QPushButton::clicked, [&]()
            {
        // fg
        QString color = selectColor();
        if (color.isEmpty())return;
        ui->label_25->setText(color);
        ui->label_24->setStyleSheet(
            QString("background-color: %1;color: %2;").arg(ui->label_24->text()).arg(color));
        ui->label_25->setStyleSheet(
            QString("background-color: %1;color: %2;").arg(ui->label_24->text()).arg(color)); });
    // tray
    connect(ui->pushButton_19, &QPushButton::clicked, [&]()
            {
        QString color = selectColor();
        if (color.isEmpty())return;
        ui->label_34->setText(color);
        ui->label_34->setStyleSheet(QString("color: %1;").arg(color)); });
    connect(ui->pushButton_20, &QPushButton::clicked, [&]()
            {
        QString color = selectColor();
        if (color.isEmpty())return;
        ui->label_35->setText(color);
        ui->label_35->setStyleSheet(QString("color: %1;").arg(color)); });
    // bool
    connect(ui->checkBox, &QCheckBox::clicked, [&]()
            { onCheckBox1Clicked(!ui->checkBox->isChecked()); });
    connect(ui->checkBox_12, &QCheckBox::clicked, [&]()
            { TrayIcon::instance()->switchMusicIcon(ui->checkBox_12->isChecked()); });
    // TTS
    connect(ui->comboBox_4, &QComboBox::currentTextChanged, this, &SettingWidget::onTTSProviderChanged);
    connect(ui->comboBox_7, &QComboBox::currentTextChanged, this, &SettingWidget::onTranslatorsChanged);
    connect(ui->pushButton_21, &QPushButton::clicked, [&]()
            {
                m_langClient->fetchProviders();
                m_langClient->fetchLanguages(); });
    connect(ui->pushButton_22, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.support_languages); });
    connect(ui->pushButton_23, &QPushButton::clicked, [&]()
            {
        QString provider = ui->comboBox->currentText();
        QString language = ui->comboBox_5->currentText();
        QString text = ui->lineEdit_14->text();
        if (provider.isEmpty() || language.isEmpty() || text.isEmpty())
        {
            qDebug() << "translate test value is empty";
            NotificationWidget::showNotification(tr("Warning"), tr("Please fill in all fields first."), 5000, MessageType::Warning);
        }
        else
        {
            m_langClient->testTranslation(text, language, provider);
        } });
    connect(m_langClient, &PyLang::languagesReady, m_langClient, [this](const QList<QString> &langs)
            {
        qDebug() << "Languages fetch successfully";
        ui->comboBox_5->clear();
        int index = 0;
        for (const auto &lang : langs)
        {
            ui->comboBox_5->addItem(lang, index);
            index++;
        } });

    connect(m_langClient, &PyLang::providersReady, m_langClient, [this](const QList<QString> &providers)
            {
        qDebug() << "Providers fetch successfully";
        ui->comboBox->clear();
        int index = 0;
        for (const auto &provider : providers)
        {
            ui->comboBox->addItem(provider, index);
            index++;
        } });

    connect(ui->pushButton_24, &QPushButton::clicked, this, &SettingWidget::onChooseVoicevoxDict);
    connect(ui->pushButton_25, &QPushButton::clicked, this, &SettingWidget::onChooseVoicevoxModel);
    connect(ui->pushButton_26, &QPushButton::clicked, this, &SettingWidget::onTestVoicevox);
    connect(ui->pushButton_27, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.voicevox_help); });
    connect(ui->pushButton_28, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.libretranslate_guide); });

    // llama
    connect(ui->pushButton_15, &QPushButton::clicked, [&]()
            {
                // 打开文件选择对话框，支持多种文本格式（包括 .md, .txt, .json 等）
                QString path = QFileDialog::getOpenFileName(
                    this,
                    tr("选择提示词文件"),
                    QString(), // 默认目录，可设为最近使用的路径
                    tr("文本文件 (*.txt *.md *.json);;所有文件 (*)"));

                if (path.isEmpty())
                    return; // 用户取消
                ui->lineEdit_13->setText(path);
                // 直接调用单例方法加载
                bool x = LlamaClient::instance()->loadSystemPromptFromFile(path);
                if (x)
                    NotificationWidget::showNotification(tr("Information"), tr("Loaded prompt from [%1] successfully.").arg(path));
                else
                    NotificationWidget::showNotification(tr("Warning"), tr("Failed to load prompt from [%1].").arg(path), 5000, MessageType::Warning); });
    // 链接跳转
    connect(ui->pushButton_7, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.openai_edge_tts_Voice_Samples); });
    connect(ui->pushButton_16, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.openai_edge_tts_github); });
    connect(ui->pushButton_3, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.iFlytek_tts_url); });
    connect(ui->pushButton_14, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.tts_server); });
    connect(ui->pushButton_4, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.openWeather_url); });
    // git-repo
    connect(ui->pushButton_8, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.website_link); });
    // team
    connect(ui->pushButton_9, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.team_link); });
    // issues
    connect(ui->pushButton_6, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.feedback_link); });
    // text
    connect(ui->pushButton_12, &QPushButton::clicked, [&]()
            { initUserTextFile(); });
    //.
    connect(ui->pushButton_13, &QPushButton::clicked, [&]()
            { launchByPath("."); });
    // voiceFolder
    connect(ui->pushButton_5, &QPushButton::clicked, [&]()
            { launchByPath(DataManager::instance().const_config_data.VoiceFolder); });
    // reset
    connect(ui->pushButton_11, &QPushButton::clicked, this, &SettingWidget::resetSetting);
    // save
    connect(ui->pushButton, &QPushButton::clicked, [&]()
            {
        saveData();
        NotificationWidget::showNotification(tr("Information"), tr("保存成功！\n下次应用启动生效。")); });
    // 检查更新
    connect(ui->pushButton_10, &QPushButton::clicked, [&]()
            { m_versionChecker->checkVersionMatch(DataManager::instance().const_config_data.version); });
    connect(m_versionChecker, &VersionChecker::versionCheckResult, this, [this](const VersionCheckSummary &summary)
            {
        UpdateDialog dialog(summary, this);
        dialog.exec(); });
}

QSlider *SettingWidget::getHorizontalSlider()
{
    return ui->horizontalSlider;
}

void SettingWidget::saveData()
{
    ConfigData data = getAllValues();
    TTSConfig tts_data = getTTSConfigValue();
    OpenWeatherData weather_data = getOpenWeatherDataValue();
    LlamaData llm_data = getLlamaDataValue();
    // 保存配置
    DataManager::instance().writeData<ConfigData>(data);
    DataManager::writeTTSConfig(tts_data);
    DataManager::writeOpenWeatherData(weather_data);
    DataManager::writeLlamaData(llm_data);
    qDebug() << "save data success";
}

bool SettingWidget::checkStartupLink()
{
    // 获取启动文件夹路径
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString appData = env.value("APPDATA");

    if (appData.isEmpty())
    {
        appData = env.value("USERPROFILE");
        if (appData.isEmpty())
        {
            qWarning() << "cannot get startup folder path";
            return false;
        }
        appData.append("/AppData/Roaming");
    }

    QString startupFolder = appData + "/Microsoft/Windows/Start Menu/Programs/Startup";

    // 获取当前应用程序路径
    QString executablePath = QCoreApplication::applicationFilePath();
    if (executablePath.isEmpty())
    {
        qWarning() << "cannot get application path";
        return false;
    }

    // 构建快捷方式名称和路径
    QString shortcutName = QFileInfo(executablePath).baseName() + ".lnk";
    QString shortcutPath = QDir(startupFolder).filePath(shortcutName);
    if (!QFile::exists(shortcutPath))
    {
        qDebug() << "shortcut not exists: " << shortcutPath;
        return false;
    }
    return true;
}

void SettingWidget::resetSetting()
{
    const auto re = QMessageBox::question(
        this, tr("Confirmation"), tr("确定要重置设置吗？\n重置后将恢复默认设置！"),
        QMessageBox::Yes | QMessageBox::No);
    if (re == QMessageBox::No)
        return;
    const ConfigData new_data = ConfigData();
    setAllValues(new_data);
    DataManager::instance().writeData<ConfigData>(new_data);
    // 重置启动项（移除）
    onCheckBox1Clicked(true);
    qDebug() << "reset setting success";
    NotificationWidget::showNotification(tr("Information"), tr("重置设置成功！"));
}

void SettingWidget::onCheckBox1Clicked(const bool flag)
{
    // 获取启动文件夹路径
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString appData = env.value("APPDATA");

    if (appData.isEmpty())
    {
        appData = env.value("USERPROFILE");
        if (appData.isEmpty())
        {
            qWarning() << "cannot get startup folder path";
        }
        appData.append("/AppData/Roaming");
    }

    QString startupFolder = appData + "/Microsoft/Windows/Start Menu/Programs/Startup";

    // 获取当前应用程序路径
    QString executablePath = QCoreApplication::applicationFilePath();
    if (executablePath.isEmpty())
    {
        qWarning() << "cannot get application path";
    }

    // 构建快捷方式名称和路径
    QString shortcutName = QFileInfo(executablePath).baseName() + ".lnk";
    QString shortcutPath = QDir(startupFolder).filePath(shortcutName);
    if (!ui->checkBox->isChecked() || flag)
    {
        // 移除启动项
        if (!QFile::exists(shortcutPath))
        {
            qDebug() << "shortcut not exists: " << shortcutPath;
            NotificationWidget::showNotification(tr("Information"), tr("快捷方式已不存在！"));
            return;
        }
        bool success = QFile::remove(shortcutPath);
        if (!success)
        {
            qWarning() << "cannot remove shortcut: " << shortcutPath;
            NotificationWidget::showNotification(tr("Warning"), tr("移除快捷方式失败！"), 5000, MessageType::Warning);
            return;
        }
        qDebug() << "remove shortcut success: " << shortcutPath;
        NotificationWidget::showNotification(tr("Information"), tr("快捷方式已移除！"));
    }
    else
    {
        // 添加启动项
        IShellLink *pShellLink = nullptr;
        IPersistFile *pPersistFile = nullptr;

        HRESULT hr = CoInitialize(nullptr);
        if (SUCCEEDED(hr))
        {
            hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink,
                                  (void **)&pShellLink);
            if (SUCCEEDED(hr))
            {
                pShellLink->SetPath(executablePath.toStdWString().c_str());
                pShellLink->SetWorkingDirectory(QFileInfo(executablePath).absolutePath().toStdWString().c_str());
                hr = pShellLink->QueryInterface(IID_IPersistFile, (void **)&pPersistFile);
                if (SUCCEEDED(hr))
                {
                    hr = pPersistFile->Save(shortcutPath.toStdWString().c_str(), TRUE);
                    pPersistFile->Release();
                }
                pShellLink->Release();
            }
            CoUninitialize();
        }

        if (FAILED(hr))
        {
            qWarning() << "cannot create shortcut: " << shortcutPath;
            NotificationWidget::showNotification(tr("Warning"), tr("创建快捷方式失败！"), 5000, MessageType::Warning);
        }
        else
        {
            qDebug() << "create shortcut success: " << shortcutPath;
            NotificationWidget::showNotification(tr("Information"), tr("快捷方式已创建！"));
        }
    }
}

QString SettingWidget::selectColor()
{
    // 创建颜色选择对话框
    QColorDialog dialog;
    dialog.setOption(QColorDialog::ShowAlphaChannel, true); // 启用透明度选项
    dialog.setCurrentColor(QColor(255, 0, 0, 128));         // 设置初始颜色（半透明红色）

    // 显示对话框并获取结果
    if (dialog.exec() == QDialog::Accepted)
    {
        QColor selectedColor = dialog.currentColor();
        QString colorStr = selectedColor.name(QColor::HexArgb);
        qDebug() << "chose color: " << colorStr;
        return colorStr;
    }
    return "";
}
void SettingWidget::onTranslatorsChanged()
{
    int index = ui->comboBox_7->currentIndex();
    qDebug() << "Translator changed to " << index;
    QList<QGroupBox *> groupBoxes = {
        ui->groupBox_5, // libretranslate
        ui->groupBox,   // translators
    };
    for (int i = 0; i < groupBoxes.size(); i++)
    {
        if (i == index)
        {
            groupBoxes[i]->setVisible(true);
        }
        else
        {
            groupBoxes[i]->setVisible(false);
        }
    }
};

void SettingWidget::onTTSProviderChanged()
{
    int index = ui->comboBox_4->currentIndex();
    qDebug() << "TTS provider changed to " << index;
    QList<QGroupBox *> groupBoxes = {
        ui->groupBox_2,
        ui->groupBox_3,
        ui->groupBox_4,
    };
    for (int i = 0; i < groupBoxes.size(); i++)
    {
        if (i == index)
            groupBoxes[i]->setVisible(true);
        else
            groupBoxes[i]->setVisible(false);
    }
}
void SettingWidget::onLogLevelChanged()
{
    LogLevel level = static_cast<LogLevel>(ui->comboBox_3->currentData().toInt());
    write_log_level(level);
    qDebug() << "set logLevel to" << static_cast<int>(level);
}

void SettingWidget::selectModelPath()
{
    QString folder = "./Resources/";
    QDir dir(folder);
    // 确保文件夹存在
    if (!dir.exists())
        QDir().mkpath(folder);
    QString path = QFileDialog::getOpenFileName(this, tr("选择模型文件"), folder, "*.model3.json");
    if (!path.isEmpty())
    {
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
// 用户点击“浏览”模型文件按钮
void SettingWidget::onChooseVoicevoxModel()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择音声模型文件"),
        "Resources/voicevox_core",
        tr("VVM 文件 (*.vvm);;所有文件 (*)"));
    if (filePath.isEmpty())
        return;

    ui->lineEdit_16->setText(filePath);

    // 通过单例加载模型
    if (!VoicevoxTTS::instance().loadModel(filePath))
    {
        NotificationWidget::showNotification(
            tr("Warning"),
            tr("模型加载失败，请检查模型文件是否正确，并重新加载。"), 5000, MessageType::Warning);
        qDebug() << "model load failed" << filePath;
        return;
    }

    refreshVoicevoxStyles();
}

// 刷新风格下拉框（comboBox_6）
void SettingWidget::refreshVoicevoxStyles()
{
    const QString previousStyleText = ui->comboBox_6->currentText();

    ui->comboBox_6->blockSignals(true);
    ui->comboBox_6->clear();

    const auto speakers = VoicevoxTTS::instance().getSpeakers();
    if (speakers.isEmpty())
    {
        ui->comboBox_6->addItem(tr("(无可用风格)"), -1);
        ui->comboBox_6->blockSignals(false);
        return;
    }

    int indexToSelect = -1;
    int currentIdx = 0;

    for (const auto &speaker : speakers)
    {
        for (const auto &style : speaker.styles)
        {
            const QString itemText = QStringLiteral("%1 - %2").arg(speaker.name, style.name);
            ui->comboBox_6->addItem(itemText, style.id);

            if (!previousStyleText.isEmpty() && itemText == previousStyleText)
            {
                indexToSelect = currentIdx;
            }
            ++currentIdx;
        }
    }

    if (indexToSelect >= 0)
    {
        ui->comboBox_6->setCurrentIndex(indexToSelect);
    }
    else if (ui->comboBox_6->count() > 0)
    {
        ui->comboBox_6->setCurrentIndex(0);
    }

    ui->comboBox_6->blockSignals(false);
}

// 实际加载辞书：更新UI并调用单例初始化
void SettingWidget::loadVoicevoxDict(const QString &dir)
{
    ui->lineEdit_15->setText(dir); // 显示路径

    if (!VoicevoxTTS::instance().initialize(dir))
    {
        qWarning() << "Failed to initialize VoicevoxTTS: " << dir;
        NotificationWidget::showNotification(tr("Warning"), tr("辞书加载失败，请检查路径是否正确，并确保该路径下有有效的字典文件"), 5000, NotificationWidget::Warning);
    }
    else
    {
        qDebug() << "[SettingWidget] Voicevox dictionary initialized successfully:" << dir;
        // 如果已加载模型，辞书重新初始化后合成器会重建，可能需要重新加载模型才能合成。
        // 可根据需求在此处自动重新加载模型或提示用户。
    }
}

// 浏览按钮槽：选择目录后调用 loadVoicevoxDict
void SettingWidget::onChooseVoicevoxDict()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("选择辞书目录"),
        "Resources/voicevox_core",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        loadVoicevoxDict(dir);
    }
}
void SettingWidget::onTestVoicevox()
{
    // 1. 直接获取当前界面的完整 TTS 配置
    TTSConfig cfg = getTTSConfigValue();
    cfg.provider = 2; // 强制使用 VOICEVOX，即使当前界面选择的是其他引擎

    // 2. 调用带配置的测试合成
    QByteArray wav = VoicevoxTTS::instance().testSynthesis(cfg);
    if (wav.isEmpty())
    {
        NotificationWidget::showNotification(tr("Warning"), tr("测试失败，请检查配置是否正确！"), 5000, MessageType ::Warning);
        return;
    }

    // 3. 保存到临时文件并播放
    QString testFile = QDir::tempPath() + "/voicevox_test.wav";
    QFile file(testFile);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(wav);
        file.close();
    }
    else
    {
        NotificationWidget::showNotification(tr("Warning"), tr("保存失败，无法保存测试音频。"), 5000, MessageType::Warning);
        return;
    }

    VoiceGenerator::instance()->playVoice(testFile);
}
SettingWidget::~SettingWidget()
{
    delete ui;
}
