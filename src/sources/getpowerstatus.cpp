/*
 * PLauncher - Live2D Virtual Desktop Assistant
 * https://github.com/Pfolg/PLauncher
 * Copyright (c) 2025 SY Cheng
 *
 * MIT License
 * https://github.com/Pfolg/PLauncher/blob/main/LICENSE
 */
#include <Windows.h>
#include <QDebug>
#include "getpowerstatus.h"

std::vector<QString> getPowerStatus() {
    SYSTEM_POWER_STATUS powerStatus;
    std::vector<QString> statusList;

    if (GetSystemPowerStatus(&powerStatus)) {
        QString ACStatus;
        switch (powerStatus.ACLineStatus) {
            case 0:

                ACStatus = "Offline (DC/Battery)";
                break;
            case 1:
                ACStatus = "Online (AC)";
                break;
            case 255:
                ACStatus = "Unknown";
                break;
            default:
                ACStatus = "Unexpected Value";
                break;
        }
        // qDebug() << ACStatus;
        // ACStatus
        statusList.push_back(ACStatus);
        // Battery Life Percent
        statusList.push_back(QString::number(static_cast<int>(powerStatus.BatteryLifePercent)));
        // Battery State
        QString BatteryFlag;
        switch (powerStatus.BatteryFlag) {
            case 1:
                BatteryFlag = "High";
                break;
            case 2:
                BatteryFlag = "Low";
                break;
            case 4:
                BatteryFlag = "Critical";
                break;
            case 8:
                BatteryFlag = "Charging";
                break;
            // case 128:
            // case 255:
            //     BatteryFlag = "No battery or unknown";
            //     break;
            default:
                // if (powerStatus.BatteryFlag == 0)
                BatteryFlag = "Unknown";
                break;
        }
        statusList.push_back(BatteryFlag);
        // qDebug() << BatteryFlag;
    }
    qDebug() << statusList[0] << statusList[1] << statusList[2];
    return statusList;
}
