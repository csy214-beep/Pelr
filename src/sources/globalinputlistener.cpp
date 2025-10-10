/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#include "globalinputlistener.h"
#include <QDebug>

GlobalInputListener *GlobalInputListener::instance = nullptr;

GlobalInputListener::GlobalInputListener(QObject *parent)
    : QObject(parent), keyboardHook(nullptr), mouseHook(nullptr) {
    instance = this;
}

GlobalInputListener::~GlobalInputListener() {
    stopListening();
    instance = nullptr;
}

bool GlobalInputListener::startListening() {
    // 安装键盘钩子
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, GetModuleHandle(nullptr), 0);
    if (!keyboardHook) {
        qWarning() << "Failed to install keyboard hook:" << GetLastError();
        return false;
    }

    // 安装鼠标钩子
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, GetModuleHandle(nullptr), 0);
    if (!mouseHook) {
        qWarning() << "Failed to install mouse hook:" << GetLastError();
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = nullptr;
        return false;
    }

    qDebug() << "Global input listening started";
    isListening = true;
    return true;
}

void GlobalInputListener::stopListening() {
    if (keyboardHook) {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = nullptr;
    }

    if (mouseHook) {
        UnhookWindowsHookEx(mouseHook);
        mouseHook = nullptr;
    }

    isListening = false;
    qDebug() << "Global input listening stopped";
}

LRESULT CALLBACK GlobalInputListener::keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        KBDLLHOOKSTRUCT *kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

        // 只在按键释放时处理
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // 获取修饰键状态
            ModifierKeys modifiers = NoModifier;
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                modifiers |= ShiftModifier;
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                modifiers |= ControlModifier;
            if (GetAsyncKeyState(VK_MENU) & 0x8000)
                modifiers |= AltModifier;
            if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000)
                modifiers |= MetaModifier;

            // 发送信号
            emit instance->keyReleased(kbStruct->vkCode, modifiers);
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK GlobalInputListener::mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        MSLLHOOKSTRUCT *msStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);

        // 获取修饰键状态
        ModifierKeys modifiers = NoModifier;
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
            modifiers |= ShiftModifier;
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            modifiers |= ControlModifier;
        if (GetAsyncKeyState(VK_MENU) & 0x8000)
            modifiers |= AltModifier;
        if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000)
            modifiers |= MetaModifier;

        // 处理鼠标移动
        if (wParam == WM_MOUSEMOVE) {
            emit instance->mouseMoved(msStruct->pt.x, msStruct->pt.y, modifiers);
        }
        // 处理鼠标按钮释放
        else if (wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP ||
                 wParam == WM_MBUTTONUP || wParam == WM_XBUTTONUP) {
            MouseButton button = NoButton;
            switch (wParam) {
                case WM_LBUTTONUP:
                    button = LeftButton;
                    break;
                case WM_RBUTTONUP:
                    button = RightButton;
                    break;
                case WM_MBUTTONUP:
                    button = MiddleButton;
                    break;
                case WM_XBUTTONUP:
                    if (GET_XBUTTON_WPARAM(msStruct->mouseData) == XBUTTON1)
                        button = XButton1;
                    else
                        button = XButton2;
                    break;
            }

            if (button != NoButton) {
                emit instance->mouseReleased(button, msStruct->pt.x, msStruct->pt.y, modifiers);
            }
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}
