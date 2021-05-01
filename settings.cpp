#include "settings.h"

const int Settings::LAUNCHER_VIEW_ICON=0;
const int Settings::LAUNCHER_VIEW_LIST=1;
const int Settings::LAUNCHER_ICON_SIZE_SMALL=0;
const int Settings::LAUNCHER_ICON_SIZE_MEDIUM=1;
const int Settings::LAUNCHER_ICON_SIZE_LARGE=2;

//******************************************************************************
// SettingsV2()
//******************************************************************************
Settings::Settings()
{
    // Set  the defaults values...
    settings["OSD_FONT_SIZE"] = QVariant(16);
    settings["OSD_COLOR"] = QVariant(QObject::tr("yellow"));
    settings["OSD_SHADOW_COLOR"] = QVariant(QObject::tr("black"));
    settings["OSD_TIMEOUT"] = QVariant(3000);
    settings["MESSAGE_TIMEOUT"] = QVariant(3000);
    settings["PLAYER_VIDEO_SMART_PAUSE"] = QVariant(true);
    settings["PLAYER_AUDIO_SMART_PAUSE"] = QVariant(false);
    settings["TITLE_BAR_TIME_FORMAT"] = QVariant(QObject::tr(" hh:mm:ss - dd/MM/yyyy"));
    settings["TITLE_BAR_POSITION_TOP"] = QVariant(true);
    settings["LAUNCHER_ICON_SIZE"] = QVariant(Settings::LAUNCHER_ICON_SIZE_MEDIUM);
    settings["LAUNCHER_VIEW"] = QVariant(Settings::LAUNCHER_VIEW_ICON);

    // ...but try to read them from user's settings
    read();
}

//******************************************************************************
// get()
//******************************************************************************
QVariant Settings::get(QString param) {
    return this->settings[param];
}

//******************************************************************************
// write()
//******************************************************************************
void Settings::write() {
    Constants *appConstants = new Constants();
    QDir appDir = QDir(QDir::homePath()).filePath(appConstants->getQString("APP_FOLDER"));
    QFile fSettings(QDir(appDir).filePath(appConstants->getQString("SETTINGS_FILE")));
    if (fSettings.open(QIODevice::WriteOnly)) {
        QDataStream out(&fSettings);
        out.setVersion(QDataStream::Qt_5_3);
        out << settings;
    }
}

//******************************************************************************
// read()
//******************************************************************************
void Settings::read() {
    Constants *appConstants = new Constants();
    QDir appDir = QDir(QDir::homePath()).filePath(appConstants->getQString("APP_FOLDER"));
    QFile fSettings(QDir(appDir).filePath(appConstants->getQString("SETTINGS_FILE")));
    if (fSettings.open(QIODevice::ReadOnly)) {
        QDataStream in(&fSettings);
        in.setVersion(QDataStream::Qt_5_3);
        in >> settings;
    }
}
