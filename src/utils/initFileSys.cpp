#include "initFileSys.h"
#include <QList>
#include <QDebug>
void initFileSys()
{
    QList<QString> paths = {
        "user",
        "log",
        "voice_files",
        "Resources",
        "Resources/voicevox_core",
    };
    for (auto &path : paths)
    {
        QDir().mkpath(path);
    }
    qDebug() << "Necessary paths have been ensured";
}