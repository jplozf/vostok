#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <QSettings>
#include <QProcess>
#include <QDir>
#include "constants.h"

class Shortcut
{
public:
    Shortcut(QString _name, QString _icon, QString _exec, QString _comment, QString _type);
    Shortcut(QString filename);
    void setName(QString);
    void setIcon(QString);
    void setExec(QString);
    void setComment(QString);
    void setType(QString);
    void write(QString);
    void launch();
    QString name;
    QString icon;
    QString exec;
    QString comment;
    QString type;
};

#endif // SHORTCUTS_H
