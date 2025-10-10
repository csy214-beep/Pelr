/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#pragma once


#include <QObject>
#include <windows.h>

// 鼠标按钮枚举
enum MouseButton {
    LeftButton,
    RightButton,
    MiddleButton,
    XButton1,
    XButton2,
    NoButton
};

// 修饰键枚举
enum ModifierKey {
    NoModifier = 0x00,
    ShiftModifier = 0x01,
    ControlModifier = 0x02,
    AltModifier = 0x04,
    MetaModifier = 0x08
};

Q_DECLARE_FLAGS(ModifierKeys, ModifierKey)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModifierKeys)

class GlobalInputListener : public QObject {
    Q_OBJECT

public:
    explicit GlobalInputListener(QObject *parent = nullptr);

    ~GlobalInputListener();

    bool isListening = false;

    bool startListening();

    void stopListening();

signals:
    void keyReleased(int keyCode, ModifierKeys modifiers);

    void mouseReleased(MouseButton button, int x, int y, ModifierKeys modifiers);

    void mouseMoved(int x, int y, ModifierKeys modifiers);

private:
    static LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

    HHOOK keyboardHook;
    HHOOK mouseHook;
    static GlobalInputListener *instance;
};

// Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalInputListener::ModifierKeys)

