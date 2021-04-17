#include "settings.h"
#include <QString>

//******************************************************************************
// Settings()
//******************************************************************************
Settings::Settings()
{
    settings["MESSAGE_TIMEOUT"] = 3000;
    settings["PLAYER_VIDEO_SMART_PAUSE"] = true;
    settings["PLAYER_AUDIO_SMART_PAUSE"] = false;
    settings["TITLE_BAR_TIME_FORMAT"] = std::string(" hh:mm:ss - dd/MM/yyyy");
    settings["TITLE_BAR_POSITION_TOP"] = true;
}

//******************************************************************************
// getInt()
//******************************************************************************
int Settings::getInt(std::string param) {
    return (std::any_cast<int>(this->settings[param]));
}

//******************************************************************************
// getBool()
//******************************************************************************
bool Settings::getBool(std::string param) {
    return (std::any_cast<bool>(this->settings[param]));
}

//******************************************************************************
// getString()
//******************************************************************************
std::string Settings::getString(std::string param) {
    return (std::any_cast<std::string>(this->settings[param]));
}

//******************************************************************************
// getQString()
//******************************************************************************
QString Settings::getQString(std::string param) {
    std::string t = std::any_cast<std::string>(this->settings[param]);
    return (QString::fromUtf8(t.c_str()));
}


