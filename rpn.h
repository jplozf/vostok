#ifndef RPN_H
#define RPN_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QStack>
#include <QMap>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <math.h>
#include <random>
#include <iomanip>
#include <iostream>
#include "constants.h"

class RPN
{
typedef int (RPN::*pfunc)(void);

public:
    QString out;
    QString entry;
    QString error;
    RPN();
    RPN(QString _entry);
    void setEntry(QString _entry);
    void initFuncs();    
    int xeq();
    QStack<double> dStack;
    QStack<QString> aStack;
    QMap<QString, pfunc> funcs;
    QMap<QString, QString> help;
    int doPlus();
    int doMinus();
    int doDivide();
    int doMultiply();
    int doSinus();
    int doCosinus();
    int doTangent();
    int doArcSinus();
    int doArcCosinus();
    int doArcTangent();
    int doPower();
    int doSquare();
    int doSquareRoot();
    int doInverse();
    int doRandom();
    int doInteger();
    int doFractional();
    int doPI();
    int doDegrees();
    int doRadians();
    int doLogNeper();
    int doExponent();
    int doLog();
    int doPower10();
    int doDup();
    int doSwap();
    int doDrop();
    int doClear();
    int doDepth();
    int doAns();
    int doAbs();
    int doChs();
    int doSign();
    int doVersion();
    int doTime();
    int doDay();
    int doMonth();
    int doYear();
    int doDate();
    int doHour();
    int doMinute();
    int doSecond();
    int doLock();
    int doQuit();
    int doShutdown();
    int doAPrompt();
    int doADrop();
    int doADup();
    int doASwap();
    int doAClear();
    int doALen();
    int doADepth();
    int doATime();
    int doADate();
    int doFix();
    int doScratchName();
    int doCls();
    QString getNext();
    QString getPrevious();
    double fRand(double fMin, double fMax);
    double ans;
    QVector <QString> entries;
    int iEntry;
    Constants *myConstants;
    int FixNumber;

    static const int RC_OK;
    static const int RC_ANS;
    static const int RC_CLS;
    static const int RC_ALIAS;
    static const int RC_COMMAND;
    static const int RC_LOCK;
    static const int RC_EXIT;
    static const int RC_PROMPT;
    static const int RC_SHUTDOWN;
    static const int RC_ERR_UNKNOWN_COMMAND;
    static const int RC_ERR_TOO_FEW_ARGUMENTS;
    static const int RC_ERR_DIVISION_BY_ZERO;
    static const int RC_ERR_BAD_ARGUMENT;
    static const QChar PREFIX_COMMAND;
    static const QChar PREFIX_ALIAS;
};

#endif // RPN_H
