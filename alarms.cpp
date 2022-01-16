#include "alarms.h"

//******************************************************************************
// Alarms()
//******************************************************************************
Alarms::Alarms() {
    Constants *myConstants = new Constants();
    QString appDir = QDir(QDir::homePath()).filePath(myConstants->getQString("APP_FOLDER"));
    QFile fDatabase(QDir(appDir).filePath(myConstants->getQString("DB_FILE")));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fDatabase.fileName());
    if(!db.open()) {
        qDebug() << "Can't Connect to DB !";
    } else {
        qDebug() << "Connected Successfully to DB !";
        QSqlQuery query;
        // query.prepare("SELECT name , phone , address FROM employees WHERE ID = 201");
        query.prepare(  "CREATE TABLE IF NOT EXISTS alarms ("
                        "enabled INTEGER,"
                        "time TEXT,"
                        "monday INTEGER,"
                        "tuesday INTEGER,"
                        "wednesday INTEGER,"
                        "thursday INTEGER,"
                        "friday INTEGER,"
                        "saturday INTEGER,"
                        "sunday INTEGER,"
                        "message TEXT(255),"
                        "sound TEXT(255)"
                        ")");

        if(!query.exec()) {
            qDebug() << "Can't Execute Query !";
        }
        /*
        } else {
            qDebug() << "Query Executed Successfully !";
            while(query.next()) {
                qDebug() << "Employee Name : " << query.value(0).toString();
                qDebug() << "Employee Phone Number : " << query.value(1).toString();
                qDebug() << "Employee Address : " << query.value(1).toString();
            }
        }
        */
    }
}

//******************************************************************************
// add()
//******************************************************************************
void Alarms::add(alarm a) {
    QSqlQuery query;
    query.prepare(  "INSERT INTO alarms (enabled, time, monday, tuesday, wednesday, thursday, friday, saturday, sunday, message, sound) "
                    "VALUES (:enabled, :time, :monday, :tuesday, :wednesday, :thursday, :friday, :saturday, :sunday, :message, :sound)");
    query.bindValue(":enabled", (a.enabled == true) ? 1 : 0);
    query.bindValue(":time", a.time);
    query.bindValue(":monday", (a.monday == true) ? 1 : 0);
    query.bindValue(":tuesday", (a.tuesday == true) ? 1 : 0);
    query.bindValue(":wednesday", (a.wednesday == true) ? 1 : 0);
    query.bindValue(":thursday", (a.thursday == true) ? 1 : 0);
    query.bindValue(":friday", (a.friday == true) ? 1 : 0);
    query.bindValue(":saturday", (a.saturday == true) ? 1 : 0);
    query.bindValue(":sunday", (a.sunday == true) ? 1 : 0);
    query.bindValue(":message", a.message);
    query.bindValue(":sound", a.sound);
    if(!query.exec()) {
        qDebug() << "Can't Execute Query !";
    }
}

//******************************************************************************
// getAlarms()
//******************************************************************************
QList<Alarms::alarm>* Alarms::getAlarms() {
    QList<alarm>* alarms = new QList<alarm>();
    QSqlQuery query;
    query.prepare("SELECT enabled, time, monday, tuesday, wednesday, thursday, friday, saturday, sunday, message, sound FROM alarms");
    if(!query.exec()) {
        qDebug() << "Can't Execute Query !";
    } else {
        qDebug() << "Query Executed Successfully !";
        while(query.next()) {
            alarm a;
            a.enabled = query.value(0).toInt();
            a.time = query.value(1).toString();
            a.monday = query.value(2).toInt();
            a.tuesday = query.value(3).toInt();
            a.wednesday = query.value(4).toInt();
            a.thursday = query.value(5).toInt();
            a.friday = query.value(6).toInt();
            a.saturday = query.value(7).toInt();
            a.sunday = query.value(8).toInt();
            a.message = query.value(9).toString();
            a.sound = query.value(10).toString();
            alarms->append(a);
        }
    }
    return alarms;
}

//******************************************************************************
// getHour()
//******************************************************************************
int Alarms::getHour(alarm a) {
    QStringList list;
    list << a.time.split(":");
    return list[0].toInt();
}

//******************************************************************************
// getMinute()
//******************************************************************************
int Alarms::getMinute(alarm a) {
    QStringList list;
    list << a.time.split(":");
    return list[1].toInt();
}

//******************************************************************************
// isEnabled()
//******************************************************************************
bool Alarms::isEnabled(alarm a) {
    return (a.enabled == 1);
}

//******************************************************************************
// isMonEnabled()
//******************************************************************************
bool Alarms::isMonEnabled(alarm a) {
    return (a.monday == 1);
}

//******************************************************************************
// isTueEnabled()
//******************************************************************************
bool Alarms::isTueEnabled(alarm a) {
    return (a.tuesday == 1);
}

//******************************************************************************
// isWedEnabled()
//******************************************************************************
bool Alarms::isWedEnabled(alarm a) {
    return (a.wednesday == 1);
}

//******************************************************************************
// isThuEnabled()
//******************************************************************************
bool Alarms::isThuEnabled(alarm a) {
    return (a.thursday == 1);
}

//******************************************************************************
// isFriEnabled()
//******************************************************************************
bool Alarms::isFriEnabled(alarm a) {
    return (a.friday == 1);
}

//******************************************************************************
// isSatEnabled()
//******************************************************************************
bool Alarms::isSatEnabled(alarm a) {
    return (a.saturday == 1);
}

//******************************************************************************
// isSunEnabled()
//******************************************************************************
bool Alarms::isSunEnabled(alarm a) {
    return (a.sunday == 1);
}

//******************************************************************************
// getSound()
//******************************************************************************
QString Alarms::getSound(alarm a) {
    return (a.sound);
}

//******************************************************************************
// getMessage()
//******************************************************************************
QString Alarms::getMessage(alarm a) {
    return (a.message);
}

/*
QString path = "path";
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");//not dbConnection
db.setDatabaseName(path);
db.open();
QSqlQuery query;
query.exec("create table if not exists person "
          "(id integer primary key, "
          "firstname varchar(20), "
          "lastname varchar(30), "
          "age integer)");
*/
