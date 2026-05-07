
#ifndef GETPOWERSTATUS_H
#define GETPOWERSTATUS_H
#include <vector>
#include <QString>

std::vector<QString> getPowerStatus();

bool isSystemUptimeExceeds(const int thresholdMinutes);
#endif // GETPOWERSTATUS_H
