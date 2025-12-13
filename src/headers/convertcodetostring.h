/*
* PLauncher - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/plauncher
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QKeySequence>
#include "globalinputlistener.h"
#include <QObject>

QString keyCodeToKeyString(int keyCode);

QString modifiersToString(ModifierKeys modifiers);

QString mouseCodeToString(MouseButton &button);
