/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "LAppDelegate.hpp"//一定要第一个导入
#include "LAppView.hpp"
#include "LAppPal.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppModel.hpp"//模型加载-表情-动作控制
#include "LAppDefine.hpp"
#include <QMouseEvent>
#include <QtCore/Qt>
#include "GLCore.h"
#include "MediaPlayerWidget.h"
#include <complex>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QCoreApplication>
#include <QWindow>
#include <QScreen>
#include <QApplication>
#include <QPushButton>
#include "BubbleBox.h"
#include "convertcodetostring.h"
#include  "getpowerstatus.h"
#include "weathermanager.h"
#include "todoNotify.hpp"
#include "ExtraMotionManager.h"
#include  "launcherMenu.hpp"

#define RECORD_FILE "user/record.dat"
#define TODO_FEATURE_MSG "暂时不支持这个功能哟，试试别的功能吧！"

GLCore::GLCore(QWidget *parent) : QOpenGLWidget(parent) {
    timer = new QTimer();
    inputCheckTimer = new QTimer();
    listener = new GlobalInputListener();
    contextMenu = new QMenu(this);
    menuWidget = new MenuWidget();
    keyLabel = new KeyLabelWidget();
    modelChatBox = new ChatBoxOnModel();
    // keyCounterTimer = new QTimer();
    main_widget = new mainWidget();
    // main_widget->show();

    // 读取数据
    // recorder = new Recorder();
    // recorder->readBinaryData(RECORD_FILE, keyCounter.first, keyCounter.second);


    // 窗口标志
    if (DataManager::instance().getBasicData().isTop) {
        this->setWindowFlags(
                Qt::FramelessWindowHint |
                Qt::WindowStaysOnTopHint |
                Qt::Tool |
                Qt::WindowTransparentForInput |
                Qt::WindowDoesNotAcceptFocus
        );
    } else {
        this->setWindowFlags(
                Qt::FramelessWindowHint |
                Qt::Tool |
                Qt::WindowTransparentForInput |
                Qt::WindowDoesNotAcceptFocus
        );
    }
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除

    connectSignals();
    //鼠标跟踪
    if (DataManager::instance().getBasicData().isLookingMouse) {
        qDebug() << "开启鼠标跟踪";
        this->setMouseTracking(true);
    }
    int step = DataManager::instance().getBasicData().model_size; //150;
    resize(4 * step, 3 * step);
    initContextMenu();
    keyLabel->show();
    BubbleBox::instance()->show();
    move(1400, 300);
}

void GLCore::checkMouseTransparency() {
    // 获取当前鼠标位置
    QPoint globalPos = QCursor::pos();
    QPoint localPos = mapFromGlobal(globalPos);
    if (DataManager::instance().getBasicData().isLookingMouse) {
        LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(localPos.x(), localPos.y());
    }
    // 如果鼠标在窗口外，不处理
    if (!rect().contains(localPos) || !isAllowDrag) {
        if (m_currentlyTransparent) {
            switchWindowTransparent(false);
            m_currentlyTransparent = false;
        }
        return;
    }
    // 鼠标位置的像素
    QImage image = grabFramebuffer();
    QColor color = image.pixelColor(localPos);

    // 检查透明度
    bool shouldBeTransparent = (color.alpha() < 64); // 25%透明度阈值

    // 如果需要改变窗口的穿透状态
    if (shouldBeTransparent != m_currentlyTransparent) {
        switchWindowTransparent(shouldBeTransparent);
        m_currentlyTransparent = shouldBeTransparent;
    }
}


void GLCore::switchWindowTransparent(bool transparent) {
    //1 穿透 0 不穿透
    // 获取当前窗口的 QWindow 句柄
    QWindow *windowHandle = this->windowHandle();
    if (!windowHandle) {
        // 窗口句柄可能尚未创建，例如在窗口显示之前
        // 可以考虑延迟执行或使用其他方法
        qCritical() << "Failed to get QWindow handle. Window may not have been created yet.";
        return;
    }

    // 获取当前的窗口标志
    Qt::WindowFlags currentFlags = windowHandle->flags();

    if (transparent) {
        // 添加穿透标志
        currentFlags |= Qt::WindowTransparentForInput;
        currentFlags |= Qt::WindowDoesNotAcceptFocus;
    } else {
        // 移除穿透标志
        currentFlags &= ~Qt::WindowTransparentForInput;
        currentFlags &= ~Qt::WindowDoesNotAcceptFocus;
    }

    // 应用新的标志
    windowHandle->setFlags(currentFlags);

    // 建议调用 update() 来请求重绘，而不是 hide()/show():cite[3]:cite[5]
    this->update();
}

void GLCore::initContextMenu() {
    qInfo() << "init context menu";
    // 实时显示键盘and鼠标按键状态 switch on/off
    keyLabel->updateWindowLocation(this->x(), this->y(), width(), height());
    QPushButton *switchListenerButton = new QPushButton("按键监听", this);
    connect(switchListenerButton, &QPushButton::clicked, this, &GLCore::switchListener);

    // 聊天
    QPushButton *RandomSentenceButton = new QPushButton("说点什么", this);
    connect(RandomSentenceButton, &QPushButton::clicked, [&]() {
        BubbleBox::instance()->RandomSentence();
    });

    //启动 把Quick Tray的功能移植到这里
    QPushButton *QuickStartButton = new QPushButton("启动", this);
    launcherMenu *launcher_menu = launcherMenu::instance(this);
    QuickStartButton->setMenu(launcher_menu);


    //询问天气（按钮）/询问电源状态/询问按键数（废弃）
    QMenu *QuestionMenu = new CustomMenu(this);
    /*
        QAction *askKeyCounter = new QAction("按键数", QuestionMenu);
        connect(askKeyCounter, &QAction::triggered, [&]() {
            bubbleBox->textSet(
                "自" + recorder->getFileCreationTimeWindows(RECORD_FILE)
                + "以来，主人一共按下键盘 " + QString::number(keyCounter.first)
                + " 次，点击鼠标 " + QString::number(keyCounter.second) + " 次。");
        });
        */
    QAction *askWeather = new QAction("天气", QuestionMenu);
    connect(askWeather, &QAction::triggered, [&]() {
        onAskWeather();
    });
    QAction *askPowerStatus = new QAction("电源状态", QuestionMenu);
    connect(askPowerStatus, &QAction::triggered, [&]() {
        std::vector<QString> powerStatus = getPowerStatus();
        if (!powerStatus.empty()) {
            BubbleBox::instance()->textSet(
                    "主人，这是您电脑目前的电源状态：\nAC: " + powerStatus[0]
                    + "\nPercentage: " + powerStatus[1]
                    + "%\nBattery State: " + powerStatus[2]);
        }
    });
    //询问最近一次待办事项
    QAction *askLatestNextTodoEvent = new QAction("TODO", QuestionMenu);
    connect(askLatestNextTodoEvent, &QAction::triggered, [&]() { TodoNotify::instance().askLatestNextEvent(); });
    QuestionMenu->addActions({askLatestNextTodoEvent, askWeather, askPowerStatus});
    QPushButton *QuestionButton = new QPushButton("问个问题", this);
    QuestionButton->setMenu(QuestionMenu);

    //设置界面
    QPushButton *SettingButton = new QPushButton("界面", this);
    connect(SettingButton, &QPushButton::clicked, [&]() {
        if (main_widget->isHidden()) {
            main_widget->show(); // 显示界面
            BubbleBox::instance()->hide();
            menuWidget->hide();
            modelChatBox->hide();
            // 更新数据
            main_widget->Widget_Todo->loadAllData();
            qDebug() << "show main_widget ";
        } else {
            main_widget->hide(); // 隐藏界面
            qDebug() << "hide main_widget ";
        }
    });
    //表情/动作控制菜单
    QPushButton *EmotionButton = new QPushButton("EMO", this);
    EmotionButton->setMenu(ExtraMotionManager::getInstance());

    //媒体播放
    QPushButton *MediaButton = new QPushButton("媒体播放", this);
    connect(MediaButton, &QPushButton::clicked, this, &GLCore::onPlayMedia);
    // 以一定次序添加按钮
    menuWidget->mainLayout->addWidget(SettingButton);
    menuWidget->mainLayout->addWidget(EmotionButton);
    menuWidget->mainLayout->addWidget(QuickStartButton);
    menuWidget->mainLayout->addWidget(switchListenerButton);
    menuWidget->mainLayout->addWidget(MediaButton);
    menuWidget->mainLayout->addWidget(RandomSentenceButton);
    menuWidget->mainLayout->addWidget(QuestionButton);

    // 启动启动项
    if (DataManager::instance().getBasicData().isStartStar) {
        qDebug() << "启动Star类下项目";
        startRunStarIfPoweredInThread();
    }
}


void GLCore::connectSignals() {
    // 更新频率
    connect(timer, &QTimer::timeout, this, [=]() {
        update();
        keyLabel->updateWindowLocation(x(), y(), width(), height());
        BubbleBox::instance()->updateWindowLocation(x(), y(), width(), height());
        modelChatBox->updateWindowLocation(x(), y(), width(), height());
        if (DataManager::instance().getBasicData().isHourAlarm) {
            // qDebug() << "user: 开启报时功能";
            BubbleBox::instance()->showTime();
        }
        TodoNotify::instance().todoNotify();
    });
    timer->start((1.0 / DataManager::instance().getBasicData().FPS) * 1000.0); // 30fps
    // 鼠标移动
    connect(listener, &GlobalInputListener::mouseMoved, [&](int x, int y) {
        keyLabel->on_mouseMove(x, y);
    });
    // 键盘按键
    connect(listener, &GlobalInputListener::keyReleased, [&](int keyCode, ModifierKeys modifiers) {
        QString keyName = keyCodeToKeyString(keyCode);
        QString modifiersName = modifiersToString(modifiers);
        // qDebug() << "Key:" << keyName << "Modifiers:" << modifiersName;
        keyLabel->on_keyRelease(keyName, modifiersName);
        // keyCounter.first++;
    });

    // 鼠标按键
    connect(listener, &GlobalInputListener::mouseReleased,
            [&](MouseButton button, int x, int y, ModifierKeys modifiers) {
                QString buttonName = mouseCodeToString(button);
                QString modifiersName = modifiersToString(modifiers);
                // qDebug() << "Mouse button:" << buttonName << "at (" << x << "," << y << ") Modifiers:" << modifiersName;
                keyLabel->on_keyRelease(buttonName, modifiersName);
                // keyCounter.second++;
            });
    //定时检查鼠标透明度
    inputCheckTimer->setInterval(50);
    connect(inputCheckTimer, &QTimer::timeout, this, &GLCore::checkMouseTransparency);

    inputCheckTimer->start();
    if (DataManager::instance().getBasicData().isListening) {
        qDebug() << "启用键盘鼠标监听";
        // 开始监听
        if (!listener->startListening()) {
            qCritical() << "Failed to start global input listening";
        } else {
            qDebug() << "Global input listener is running. ";
        }
    }
    //定时说话
    randomSentenceTimer = new QTimer();
    randomSentenceTimer->setSingleShot(true);
    connect(randomSentenceTimer, &QTimer::timeout, [&]() {
        int randomTime = rand() % 10 + 25; //15-25min
        qInfo() << "next random sentence in " << QString::number(randomTime * 60 * 1000) << " s";
        BubbleBox::instance()->RandomSentence();
        randomSentenceTimer->start(randomTime * 60 * 1000);
    });
    if (DataManager::instance().getBasicData().isRandomSpeech) {
        qDebug() << "启用随机说话";
        randomSentenceTimer->start(10 * 60 * 1000); //3min
    }
    /*
        keyCounterTimer->setInterval(60000); // 1min
        connect(keyCounterTimer, &QTimer::timeout, [&]() {
                    recorder->writeBinaryData(RECORD_FILE, keyCounter.first, keyCounter.second);
                }
        );
        keyCounterTimer->start();
        */
    // 实时预览模型
    // load model //void SettingWidget::selectModelPath()
    //  connect(main_widget->Widget_Setting->ui->lineEdit, &QLineEdit::textChanged, this, &GLCore::loadModel);
    // size
    connect(main_widget->Widget_Setting->getHorizontalSlider(), &QSlider::valueChanged, [&]() {
        int var = main_widget->Widget_Setting->getHorizontalSlider()->value();
        resize(4 * var, 3 * var);
    });
}

void GLCore::switchListener() {
    if (listener->isListening) {
        listener->stopListening();
        BubbleBox::instance()->textSet("按键监听已关闭");
    } else {
        listener->startListening();
        BubbleBox::instance()->textSet("按键监听已开启");
    }
}

void GLCore::silentMode() {
    if (isSilentMode) {
        show();
        timer->start();
        inputCheckTimer->start();
        if (DataManager::instance().getBasicData().isRandomSpeech) randomSentenceTimer->start();
        if (DataManager::instance().getBasicData().isListening) listener->startListening();
        qDebug() << "silent mode off";
        isSilentMode = false;
    } else {
        hide();
        timer->stop();
        inputCheckTimer->stop();
        randomSentenceTimer->stop();
        BubbleBox::instance()->hide();
        menuWidget->hide();
        modelChatBox->hide();
        qDebug() << "silent mode on";
        isSilentMode = true;
    }
}


void GLCore::onAskWeather() {
    // 获取单例实例
    WeatherManager *weatherManager = WeatherManager::instance();

    // API Key和城市名
    QString apiKey = DataManager::instance().getBasicData().api_key;
    QString city = DataManager::instance().getBasicData().city;

    // 调用单例方法获取天气数据
    WeatherData weather = weatherManager->getWeatherData(city, apiKey);
    QString msg;
    if (weather.error.isEmpty()) {
        /*
        qDebug() << "城市：" << weather.city;
        qDebug() << "温度：" << weather.temperature << "℃";
        qDebug() << "天气：" << weather.description;
        qDebug() << "湿度：" << weather.humidity << "%";*/
        msg = weather.city + "，" + QString::number(weather.temperature) + "℃，" + weather.description + "，湿度：" +
              weather.humidity + "%。";
    } else {
        // qDebug() <<__func__<< "错误：" << weather.error;
        msg = weather.error;
    }
    qDebug() << msg;
    BubbleBox::instance()->textSet(msg);
}

void GLCore::startRunStarIfPoweredInThread() {
    // 设置完成信号与槽的连接
    connect(&m_watcher, &QFutureWatcher<void>::finished, this, &GLCore::onRunStarIfPoweredFinished);

    // 启动任务，运行在子线程中
    QFuture<void> future = QtConcurrent::run(this, &GLCore::runStarIfPowered);
    m_watcher.setFuture(future);
}

void GLCore::onPlayMedia() {
    MediaPlayerWidget::instance().setVisible(!MediaPlayerWidget::instance().isVisible());
    qDebug() << "MediaPlayerWidget visible: " << MediaPlayerWidget::instance().isVisible();
}


void GLCore::onRunStarIfPoweredFinished() {
    // 任务完成后进行清理
    m_watcher.disconnect();
    qDebug() << "Task completed and resources cleaned.";
}

void GLCore::runStarIfPowered() {
    qDebug() << "run star if powered，wt 60s";
    QThread::sleep(60); // 等待60s
    QList<MenuData> menu_data = DataManager::instance().getMenuData();
    std::vector<QString> powerStatus = getPowerStatus();
    if (powerStatus[0] == "Online (AC)") {
        for (MenuData &item: menu_data) {
            if (item.category == "Star") {
                launchByPath(item.path);
                QThread::sleep(3);
            }
        }
    }
}


void GLCore::switchDragStatus() {
    isAllowDrag = !isAllowDrag;
    if (isAllowDrag) {
        qDebug() << "允许窗口拖动";
    } else {
        qDebug() << "禁止窗口拖动";
        switchWindowTransparent(true);
    }
}

void GLCore::onExpMotionSelected() {
}


//unused
void GLCore::loadModel() {
    QString model_path = DataManager::instance().getBasicData().model_path;
    // QString msg;
    if (model_path.isEmpty()) {
        qCritical() << "model path is empty";
        // msg = "模型路径为空，请在设置中设置模型路径！";
        model_path = "Resources/Hiyori/Hiyori.model3.json";
    }
    QFileInfo file_info(model_path);
    if (!file_info.exists()) {
        qCritical() << "model file not exists: " << model_path;
        // msg = "模型文件不存在！";
        model_path = "Resources/Hiyori/Hiyori.model3.json";
        file_info.setFile(model_path);
    }
    QString dir = file_info.absolutePath();
    QString fileName = file_info.fileName();
    qDebug() << "model dir: " << dir << " fileName: " << fileName;
    LAppLive2DManager::GetInstance()->LoadModelFromPath(dir.toStdString() + "/", fileName.toStdString());
    // if (!msg.isEmpty()) QMessageBox::critical(this, "Error", msg);
}

void GLCore::resetLocation() {
    QCoreApplication *coreapp = QCoreApplication::instance();
    QApplication *app = qobject_cast<QApplication *>(coreapp);
    QScreen *screen = app->screens().at(0);
    QRect screenRect = screen->geometry();
    int x = (screenRect.width() - width()) / 2;
    int y = (screenRect.height() - height()) / 2;
    int step = DataManager::instance().getBasicData().model_size;
    resize(4 * step, 3 * step);
    move(x, y);
}

void GLCore::mousePressEvent(QMouseEvent *event) {
    // LAppDelegate::GetInstance()->GetView()->OnTouchesBegan(event->x(), event->y());
    // 按住鼠标开始拖动
    if (event->button() == Qt::LeftButton) {
        left_button_down = true;
        dragStartPos = event->globalPos() - frameGeometry().topLeft();
    }


    if (event->button() == Qt::RightButton) {
        right_button_down = true;
        if (menuWidget->isHidden()) {
            menuWidget->showNearMouse();
        } else {
            menuWidget->hide();
        }
    }
    if (event->button() == Qt::MiddleButton) {
        event->accept();
    }
}

void GLCore::mouseMoveEvent(QMouseEvent *event) {
    LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(event->x(), event->y());
    // 按住鼠标移动时拖动窗口
    if (left_button_down && (event->buttons() & Qt::LeftButton) && isAllowDrag) {
        move(event->globalPos() - dragStartPos);
        // keyLabel->updateWindowLocation(this->x(), this->y(), width(), height());
    }
}

void GLCore::mouseReleaseEvent(QMouseEvent *event) {
    LAppDelegate::GetInstance()->GetView()->OnTouchesEnded(event->x(), event->y());
    // 按住鼠标释放时停止拖动
    if (event->button() == Qt::LeftButton) {
        left_button_down = false;
    }
    if (event->button() == Qt::RightButton) {
        right_button_down = false;
    }
}

void GLCore::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        modelChatBox->setVisible(!modelChatBox->isVisible());
        qDebug() << "model chat box visible: " << modelChatBox->isVisible();
    }
}


void GLCore::wheelEvent(QWheelEvent *event) {
}


void GLCore::initializeGL() {
    LAppDelegate::GetInstance()->Initialize(this);
}

void GLCore::paintGL() {
    LAppDelegate::GetInstance()->update();
}

void GLCore::resizeGL(int w, int h) {
    LAppDelegate::GetInstance()->resize(w, h);
}
