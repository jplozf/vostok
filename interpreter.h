#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QStack>
#include <QDir>
#include <QDateTime>
#include <QProcess>
#include <cmath>
#include "constants.h"
#include "settings.h"

class Interpreter
{
public:
    Interpreter();
    Interpreter(QString _command);
    void setCommand(QString _command);
    QString xeq();
    QString xeq(QString _command);
    QString command;
    QStack<long double> numstack;
    int rc;
    int getRC();
    static const int RC_OK;
    static const int RC_OFF;
    static const int RC_EXIT;

private:
    QStringList tokens;
    QString pop();
};

#endif // INTERPRETER_H
