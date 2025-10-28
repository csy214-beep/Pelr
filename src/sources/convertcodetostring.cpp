/*
* PLauncher - Live2D Virtual Desktop Assistant
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#include "globalinputlistener.h"

// 键码到按键名称的映射表
QString keyCodeToKeyString(int keyCode) {
    switch (keyCode) {
        case 8:
            return "Backspace";
        case 9:
            return "Tab";
        case 13:
            return "Enter";
        case 20:
            return "CapsLock";
        case 27:
            return "Escape";
        case 32:
            return "Space";
        case 33:
            return "PageUp";
        case 34:
            return "PageDown";
        case 35:
            return "End";
        case 36:
            return "Home";
        case 37:
            return "Left";
        case 38:
            return "Up";
        case 39:
            return "Right";
        case 40:
            return "Down";
        case 44:
            return "Print";
        case 45:
            return "Insert";
        case 46:
            return "Delete";
        case 65: // Qt::Key_A
            return "A";
        case 66: // Qt::Key_B
            return "B";
        case 67: // Qt::Key_C
            return "C";
        case 68: // Qt::Key_D
            return "D";
        case 69: // Qt::Key_E
            return "E";
        case 70: // Qt::Key_F
            return "F";
        case 71: // Qt::Key_G
            return "G";
        case 72: // Qt::Key_H
            return "H";
        case 73: // Qt::Key_I
            return "I";
        case 74: // Qt::Key_J
            return "J";
        case 75: // Qt::Key_K
            return "K";
        case 76: // Qt::Key_L
            return "L";
        case 77: // Qt::Key_M
            return "M";
        case 78: // Qt::Key_N
            return "N";
        case 79: // Qt::Key_O
            return "O";
        case 80: // Qt::Key_P
            return "P";
        case 81: // Qt::Key_Q
            return "Q";
        case 82: // Qt::Key_R
            return "R";
        case 83: // Qt::Key_S
            return "S";
        case 84: // Qt::Key_T
            return "T";
        case 85: // Qt::Key_U
            return "U";
        case 86: // Qt::Key_V
            return "V";
        case 87: // Qt::Key_W
            return "W";
        case 88: // Qt::Key_X
            return "X";
        case 89: // Qt::Key_Y
            return "Y";
        case 90: // Qt::Key_Z
            return "Z";
        case 48:
        case 96:
            return "0";
        case 49:
        case 97:
            return "1";
        case 50:
        case 98:
            return "2";
        case 51:
        case 99:
            return "3";
        case 52:
        case 100:
            return "4";
        case 53:
        case 101:
            return "5";
        case 54:
        case 102:
            return "6";
        case 55:
        case 103:
            return "7";
        case 56:
        case 104:
            return "8";
        case 57:
        case 105:
            return "9";
        case 106:
            return "*";
        case 107:
            return "+";
        case 109:
            return "-";
        case 110:
            return ".";
        case 111:
            return "/";
        case 112:
            return "F1";
        case 113:
            return "F2";
        case 114:
            return "F3";
        case 115:
            return "F4";
        case 116:
            return "F5";
        case 117:
            return "F6";
        case 118:
            return "F7";
        case 119:
            return "F8";
        case 120:
            return "F9";
        case 121:
            return "F10";
        case 122:
            return "F11";
        case 123:
            return "F12";
        case 144:
            return "NumLock";
        case 186:
            return ";";
        case 187:
            return "=";
        case 188:
            return ",";
        case 189:
            return "-";
        case 190:
            return ".";
        case 191:
            return "/";
        case 192:
            return "`";
        case 219:
            return "[";
        case 220:
            return "|";
        case 221:
            return "]";
        case 222:
            return "'";
        // 添加更多键码映射
        default:
            return ""; // QString::number(keyCode);
    }
}

// 修饰键到字符串的转换函数
QString modifiersToString(ModifierKeys modifiers) {
    QStringList modifierStrings;
    if (modifiers & ShiftModifier)
        modifierStrings << "Shift";
    if (modifiers & ControlModifier)
        modifierStrings << "Ctrl";
    if (modifiers & AltModifier)
        modifierStrings << "Alt";
    if (modifiers & MetaModifier)
        modifierStrings << "Meta";

    return modifierStrings.join(" + ");
}

// 鼠标按键码到字符串的转换函数
QString mouseCodeToString(MouseButton &b) {
    QString buttonName;
    switch (b) {
        case LeftButton:
            buttonName = "LeftButton";
            break;
        case RightButton:
            buttonName = "RightButton";
            break;
        case MiddleButton:
            buttonName = "MiddleButton";
            break;
        case XButton1:
            buttonName = "XButton1";
            break;
        case XButton2:
            buttonName = "XButton2";
            break;
        case NoButton:
            buttonName = "NoButton";
            break;
    }
    return buttonName;
}
