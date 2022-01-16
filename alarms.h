#ifndef ALARMS_H
#define ALARMS_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>

#include "constants.h"

class Alarms
{
public:   
    typedef struct _alarm {
        bool enabled;
        QString time;
        bool monday;
        bool tuesday;
        bool wednesday;
        bool thursday;
        bool friday;
        bool saturday;
        bool sunday;
        QString message;
        QString sound;
    } alarm;

    Alarms();
    void add(alarm a);
    QList<alarm>* getAlarms();
    int getHour(alarm a);
    int getMinute(alarm a);
    QString getMessage(alarm a);
    QString getSound(alarm a);
    bool isEnabled(alarm a);
    bool isMonEnabled(alarm a);
    bool isTueEnabled(alarm a);
    bool isWedEnabled(alarm a);
    bool isThuEnabled(alarm a);
    bool isFriEnabled(alarm a);
    bool isSatEnabled(alarm a);
    bool isSunEnabled(alarm a);

private:
    QSqlDatabase db;
};

#endif // ALARMS_H
