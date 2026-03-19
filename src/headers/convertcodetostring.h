#pragma once
#include <QString>
#include <QFlags>

enum ModifierKey {
    NoModifier = 0,
    Shift = 1 << 0,
    Ctrl  = 1 << 1,
    Alt   = 1 << 2,
    Win   = 1 << 3
};
Q_DECLARE_FLAGS(ModifierKeys, ModifierKey)
Q_DECLARE_OPERATORS_FOR_FLAGS(ModifierKeys)

enum MouseButton {
    LeftButton, RightButton, MiddleButton, XButton1, XButton2, UnknownButton
};

// 工具函数
QString keyCodeToKeyString(int keyCode);
QString modifiersToString(ModifierKeys modifiers);
QString mouseCodeToString(MouseButton button);
bool isSpecialKey(int keyCode, ModifierKeys modifiers);
bool isModifierKeyCode(int keyCode);