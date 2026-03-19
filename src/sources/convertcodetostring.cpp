#include "convertcodetostring.h"

#include <QList>
#include <windows.h>
#include <QDebug>

QString keyCodeToKeyString(int keyCode) {
    if (keyCode >= 'A' && keyCode <= 'Z') return QString(QChar(keyCode));
    if (keyCode >= '0' && keyCode <= '9') return QString(QChar(keyCode));
    if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) return QString::number(keyCode - VK_NUMPAD0);

    switch (keyCode) {
        case VK_SPACE: return "Space";
        case VK_RETURN: return "Enter";
        case VK_ESCAPE: return "Esc";
        case VK_BACK: return "Backspace";
        case VK_TAB: return "Tab";
        case VK_DELETE: return "Del";
        case VK_UP: return "Up";
        case VK_DOWN: return "Down";
        case VK_LEFT: return "Left";
        case VK_RIGHT: return "Right";
        case VK_OEM_1: return ";";
        case VK_OEM_PLUS: return "=";
        case VK_OEM_COMMA: return ",";
        case VK_OEM_MINUS: return "-";
        case VK_OEM_PERIOD: return ".";
        case VK_OEM_2: return "/";
        case VK_OEM_3: return "`";
        case VK_OEM_4: return "[";
        case VK_OEM_5: return "\\";
        case VK_OEM_6: return "]";
        case VK_OEM_7: return "'";
    }

    if (keyCode >= VK_F1 && keyCode <= VK_F24) {
        return QString("F%1").arg(keyCode - VK_F1 + 1);
    }
    return ""; // 其他键暂略或为空
}

QString modifiersToString(ModifierKeys modifiers) {
    QStringList parts;
    if (modifiers.testFlag(Ctrl)) parts << "Ctrl";
    if (modifiers.testFlag(Alt)) parts << "Alt";
    if (modifiers.testFlag(Win)) parts << "Win";
    if (modifiers.testFlag(Shift)) parts << "Shift";
    return parts.join(" + ");
}

QString mouseCodeToString(MouseButton button) {
    switch (button) {
        case LeftButton: return "Left";
        case RightButton: return "Right";
        case MiddleButton: return "Middle";
        case XButton1: return "Side1";
        case XButton2: return "Side2";
        default: return "UnknownMouseButton";
    }
}

bool isModifierKeyCode(int keyCode) {
    return (keyCode == VK_SHIFT || keyCode == VK_LSHIFT || keyCode == VK_RSHIFT ||
            keyCode == VK_CONTROL || keyCode == VK_LCONTROL || keyCode == VK_RCONTROL ||
            keyCode == VK_MENU || keyCode == VK_LMENU || keyCode == VK_RMENU ||
            keyCode == VK_LWIN || keyCode == VK_RWIN);
}

bool isSpecialKey(int keyCode, ModifierKeys modifiers) {
    // 包含修饰键的组合键视为特殊键
    if (modifiers != NoModifier && keyCode != 0) return true;
    // F区功能键、控制键视为特殊键
    if ((keyCode >= VK_F1 && keyCode <= VK_F24) ||
        keyCode == VK_ESCAPE || keyCode == VK_BACK ||
        keyCode == VK_RETURN || keyCode == VK_TAB) {
        return true;
    }
    return false;
}
