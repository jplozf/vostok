#include "shortcuts.h"

//******************************************************************************
// Shortcut()
//******************************************************************************
Shortcut::Shortcut(QString _name, QString _icon, QString _exec, QString _comment, QString _type)
    : name(_name),
      icon(_icon),
      exec(_exec),
      comment(_comment),
      type(_type)
{
    // fields are set.
}

//******************************************************************************
// Shortcut()
//******************************************************************************
Shortcut::Shortcut(QString filename) {
    QSettings shortcutFile(filename, QSettings::IniFormat);
    icon = shortcutFile.value("Icon", ":/png/png/vostok_48").toString();
    exec = shortcutFile.value("Exec").toString();
    name = shortcutFile.value("Name").toString();
    comment = shortcutFile.value("Comment").toString();
    type = shortcutFile.value("Type", "Application").toString();
}

//******************************************************************************
// setName()
//******************************************************************************
void Shortcut::setName(QString _name) {
    this->name = _name;
}

//******************************************************************************
// setIcon()
//******************************************************************************
void Shortcut::setIcon(QString _icon) {
    this->icon = _icon;
}

//******************************************************************************
// setExec()
//******************************************************************************
void Shortcut::setExec(QString _exec) {
    this->exec = _exec;
}

//******************************************************************************
// setComment()
//******************************************************************************
void Shortcut::setComment(QString _comment) {
    this->comment = _comment;
}

//******************************************************************************
// setType()
//******************************************************************************
void Shortcut::setType(QString _type) {
    this->type = _type;
}

//******************************************************************************
// launch()
//******************************************************************************
void Shortcut::launch() {
    if (this->type == "Application") {
        QString file(this->exec);
        QProcess::startDetached(file);
    } else {
        if (this->type == "Application in terminal") {
            // FIXME : Open the terminal
            QString file(this->exec);
            QProcess::startDetached(file);
        } else {
#ifdef Q_OS_LINUX
            QString file(this->exec);
            QProcess::startDetached("xdg-open", QStringList(file));
#endif
#ifdef Q_OS_WIN32
            // TODO : Open file
#endif
        }
    }
}

//******************************************************************************
// write()
//******************************************************************************
void Shortcut::write(QString target) {
    Constants *myConstants = new Constants();
    QString shortcutFileName =  QDir(target).filePath(this->name).append("." + myConstants->getQString("LAUNCHER_SUFFIX"));
    QSettings shortcutFile(shortcutFileName, QSettings::IniFormat);
    shortcutFile.setValue("Icon", this->icon);
    shortcutFile.setValue("Exec", this->exec);
    shortcutFile.setValue("Name", this->name);
    shortcutFile.setValue("Comment", this->comment);
    shortcutFile.setValue("Type", this->type);
}

/*
 * QSettings settings("/home/user/conf.conf", QSettings::IniFormat);
 * QString value1 = settings.value("Key1").toString();
 * On Linux in most of the cases conf-file is the same format as ini-file:
            key = value

#!/usr/bin/env xdg-open
[Desktop Entry]
Version=1.0
Type=Application
Terminal=false
Icon=/home/jpl/bin/molotov.png
Icon[fr_FR]=/home/jpl/bin/molotov.png
Exec=/home/jpl/bin/molotov
Name[fr_FR]=Molotov
Comment[fr_FR]=Molotov TV Viewer
Name=Molotov
Comment=Molotov TV Viewer

*/

