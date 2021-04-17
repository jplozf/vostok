#ifndef SETTINGS_H
#define SETTINGS_H

#include <unordered_map>
#include <string>
#include <any>
#include <iostream>
#include <vector>
#include <QString>


class Settings
{
public:
    Settings();
    std::unordered_map<std::string, std::any> settings;
    int getInt(std::string param);
    bool getBool(std::string param);
    QString getQString(std::string param);
    std::string getString(std::string param);
};

#endif // SETTINGS_H
