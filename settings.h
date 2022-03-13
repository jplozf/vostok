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
#include <QPair>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QWidget>

#include "rpn.h"
#include "constants.h"

class Settings: public QObject
{
    Q_OBJECT
public:
    static const int LAUNCHER_VIEW_ICON;
    static const int LAUNCHER_VIEW_LIST;
    static const int LAUNCHER_ICON_SIZE_SMALL;
    static const int LAUNCHER_ICON_SIZE_MEDIUM;
    static const int LAUNCHER_ICON_SIZE_LARGE;

    Settings();
    QMap<QString, QVariant> defaults;
    QMap<QString, QVariant> settings;
    QVariant get(QString param);
    void write();
    void read();
    void form(QWidget *);
    void handleTextChanged(QLabel *lbl, QLineEdit *txt);
};

#endif // SETTINGS_H
