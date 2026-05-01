#include "data.hpp"

void DataManager::writeOpenWeatherData(const OpenWeatherData &opwdt)
{
    QJsonObject json_object;
    json_object.insert("city", opwdt.city);
    json_object.insert("api_key", opwdt.api_key);
    QFile file(OPEN_WEATHER_FILE);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败：%1").arg(OPEN_WEATHER_FILE));
        qCritical() << "write OpenWeather Config failed: can not open file";
        return; // 无法打开文件进行写入
    }
    QJsonDocument json_doc(json_object);
    file.write(json_doc.toJson());
    file.close();
}
void DataManager::writeLlamaData(const LlamaData &llm)
{
    QJsonObject json_object;
    json_object.insert("maxContextMessages", llm.maxContextMessages);
    json_object.insert("model", llm.model);
    json_object.insert("systemPrompt", llm.systemPrompt);
    json_object.insert("baseUrl", llm.baseUrl);
    json_object.insert("apiKey", llm.apiKey);
    json_object.insert("promptFilePath", llm.promptFilePath);
    QFile file(LLAMA_DATA_FILE);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败：%1").arg(OPEN_WEATHER_FILE));
        qCritical() << "write OpenWeather Config failed: can not open file";
        return; // 无法打开文件进行写入
    }
    QJsonDocument json_doc(json_object);
    file.write(json_doc.toJson());
    file.close();
}

void DataManager::writeTTSConfig(const TTSConfig &ttsc)
{
    QJsonObject json_object;
    json_object.insert("provider", ttsc.provider);
    json_object.insert("speaker_openai_edge_tts", ttsc.speaker_openai_edge_tts);
    json_object.insert("speed_openai_edge_tts", ttsc.speed_openai_edge_tts);
    json_object.insert("iFlytek_APPID", ttsc.iFlytek_APPID);
    json_object.insert("iFlytek_APISecret", ttsc.iFlytek_APISecret);
    json_object.insert("iFlytek_APIKey", ttsc.iFlytek_APIKey);
    json_object.insert("iFlytek_speaker", ttsc.iFlytek_speaker);
    json_object.insert("isRunTTSServerOnStartUp", ttsc.isRunTTSServerOnStartUp);
    json_object.insert("tr_provider", ttsc.tr_provider);
    json_object.insert("tr_lang", ttsc.tr_lang);
    QFile file(TTS_CONFIG_FILE);
    if (!file.open(QIODevice::WriteOnly))
    {
        // 无法打开文件进行写入
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("写入数据失败: %1").arg(TTS_CONFIG_FILE));
        qCritical() << "write tts config failed: can not open file";
        return;
    }
    QJsonDocument json_doc(json_object);
    file.write(json_doc.toJson());
    file.close();
}
QJsonObject DataManager::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open file:" << filePath << "错误:" << file.errorString();
        return {}; // 文件不存在或无法打开
    }

    QByteArray fileData = file.readAll();
    file.close();

    if (fileData.isEmpty())
    {
        qWarning() << "file is empty:" << LLAMA_DATA_FILE;
        return {}; // 空文件
    }

    QJsonParseError parseError;
    QJsonDocument json_doc = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qCritical() << "JSON Parse Error:" << parseError.errorString()
                    << "at offset:" << parseError.offset;
        return {}; // JSON格式错误
    }

    if (!json_doc.isObject())
    {
        qWarning() << "file root element is not an object";
        return {}; // 根元素必须是对象
    }

    return json_doc.object();
}
void DataManager::readLlamaData()
{
    QJsonObject json_obj = readJsonFile(LLAMA_DATA_FILE);

    // 逐个读取
    llama_data.maxContextMessages = json_obj.value("maxContextMessages").toInt(10);
    llama_data.model = json_obj.value("model").toString("");
    llama_data.systemPrompt = json_obj.value("systemPrompt").toString("You are a friendly AI assistant.");
    llama_data.baseUrl = json_obj.value("baseUrl").toString("");
    llama_data.apiKey = json_obj.value("apiKey").toString("");
    llama_data.promptFilePath = json_obj.value("promptFilePath").toString("");
}

void DataManager::writeData(ToDoSettingData setting)
{
    QFile file(TODO_NOTIFY_FILE);
    if (!file.open(QIODevice::WriteOnly))
    {
        // 无法打开文件进行写入
        QMessageBox::critical(nullptr, "Error", "写入数据失败！");
        qCritical() << "write todo setting data failed: can not open file";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // 设置流版本以确保兼容性

    // 写入数据
    out << setting;
    file.close();
    // 更新缓存
    todo_setting_data = setting;
}

void DataManager::readMenuData()
{
    QList<MenuData> menu_data;
    QFile file(MENU_DATA_FILE);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "menu data file not exist or can not open, return empty list";
        return; // 文件不存在或无法打开，返回空列表
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
    in >> menu_data;
    file.close();
    cached_menu_data = menu_data;
}
void DataManager::readBasicData()
{
    ConfigData _basic_data;
    QFile file(CONFIG_DATA_FILE);
    if (!file.open(QIODevice::ReadOnly))
    {
        // qDebug() <<__func__<< "读取菜单数据失败";
        return; // 文件不存在或无法打开，返回空列表
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
    in >> _basic_data;
    file.close();
    basic_data = _basic_data;
}
QFont DataManager::loadFont()
{
    QString boldFontPath = ":/public/font/MapleMono-NF-CN-Medium.ttf";
    if (QFile::exists(boldFontPath))
    {
        int boldFontId = QFontDatabase::addApplicationFont(boldFontPath);
        if (boldFontId != -1)
        {
            QStringList boldFamilies = QFontDatabase::applicationFontFamilies(boldFontId);
            if (!boldFamilies.isEmpty())
            {
                qInfo() << "load font success:" << boldFamilies.at(0);
                return QFont(boldFamilies.at(0));
            }
        }
    }
    return QFont();
}

void DataManager::readTodoData()
{
    QList<TodoData> _todo_data;
    QFile file(TODO_DATA_FILE);
    if (!file.open(QIODevice::ReadOnly))
    {
        // qDebug() <<__func__<< "读取菜单数据失败";
        return; // 文件不存在或无法打开，返回空列表
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
    in >> _todo_data;
    file.close();
    todo_data = _todo_data;
}
void DataManager::readTodoNotify()
{
    QFile file(TODO_NOTIFY_FILE);
    if (!file.open(QIODevice::ReadOnly))
    {
        // qDebug() <<__func__<< "读取菜单数据失败";
        return; // 文件不存在或无法打开，返回空列表
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_11); // 设置流版本以确保兼容性
    in >> todo_setting_data;
    file.close();
}
void DataManager::readTTSConfig()
{
    QJsonObject json_object = readJsonFile(TTS_CONFIG_FILE);

    // 逐个读取并验证必要字段
    tts_config.provider = json_object.value("provider").toInt(0);
    if (tts_config.provider < 0 || tts_config.provider > 1)
    {
        tts_config.provider = 0;
    }
    tts_config.speaker_openai_edge_tts = json_object.value("speaker_openai_edge_tts").toString("zh-CN-XiaoxiaoNeural");
    tts_config.speed_openai_edge_tts = json_object.value("speed_openai_edge_tts").toDouble(1.0);
    tts_config.iFlytek_APPID = json_object.value("iFlytek_APPID").toString("");
    tts_config.iFlytek_APISecret = json_object.value("iFlytek_APISecret").toString("");
    tts_config.iFlytek_APIKey = json_object.value("iFlytek_APIKey").toString("");
    tts_config.iFlytek_speaker = json_object.value("iFlytek_speaker").toString("x4_yezi");
    if (tts_config.iFlytek_APPID.isEmpty() || tts_config.iFlytek_APISecret.isEmpty() || tts_config.iFlytek_APIKey.isEmpty())
        tts_config.provider = 0;
    tts_config.isRunTTSServerOnStartUp = json_object.value("isRunTTSServerOnStartUp").toBool(false);
    tts_config.tr_provider = json_object.value("tr_provider").toString("");
    tts_config.tr_lang = json_object.value("tr_lang").toString("");
}
void DataManager::readOpenWeatherData()
{

    QJsonObject json_object = readJsonFile(OPEN_WEATHER_FILE);

    // 逐个读取并验证必要字段
    openWeather_data.api_key = json_object.value("api_key").toString("");
    if (openWeather_data.api_key.isEmpty())
    {
        qWarning() << "api_key in Config file is empty or not exist";
    }
    openWeather_data.city = json_object.value("city").toString("");
}