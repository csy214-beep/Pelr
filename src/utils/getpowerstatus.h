/*
 * Pelr - Live2D Virtual Desktop Partner
 * https://gitee.com/Pfolg/Pelr
 * https://sourceforge.net/projects/pfolg-plauncher/
 * Copyright (c) 2025 SY Cheng
 *
 * GPL v3 License
 * https://gnu.ac.cn/licenses/gpl-3.0.html
 */
#ifndef GETPOWERSTATUS_H
#define GETPOWERSTATUS_H
#include <vector>
#include <QString>

std::vector<QString> getPowerStatus();

bool isSystemUptimeExceeds(const int thresholdMinutes);
#endif // GETPOWERSTATUS_H
