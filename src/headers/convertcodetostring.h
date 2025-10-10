/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
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
