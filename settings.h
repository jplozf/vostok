#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QDataStream>
#include <QStandardPaths>

#include "constants.h"

class Settings
{
public:
    static const int LAUNCHER_VIEW_ICON;
    static const int LAUNCHER_VIEW_LIST;
    static const int LAUNCHER_ICON_SIZE_SMALL;
    static const int LAUNCHER_ICON_SIZE_MEDIUM;
    static const int LAUNCHER_ICON_SIZE_LARGE;

    Settings();
    QMap<QString, QVariant> settings;
    QVariant get(QString param);
    void write();
    void read();
};

#endif // SETTINGS_H
