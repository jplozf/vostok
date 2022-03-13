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
#include <chrono>
#include <iomanip>
#include <iostream>
#include "constants.h"
#include "mainwindow.h"

class RPN
{
typedef int (RPN::*pfunc)(void);

public:
    enum AngularMode
    {
       DEGREES,
       RADIANS
    };

    enum TypeStack
    {
        NUMBER,
        STRING,
        NAME,
        CODE,
        TOO_FEW_ARGS,
        BAD_TYPE_ARG
    };

    enum AtStack
    {
        TOP_0,
        TOP_1,
        TOP_2
    };

    QMainWindow *mw;
    QString out;
    QString entry;
    QString error;
    QString alpha;
    AngularMode angularMode = DEGREES;
    RPN(QMainWindow *mw);
    RPN(QMainWindow *mw, QString _entry);
    void setEntry(QString _entry);
    void initFuncs();    
    int xeq();
    QStack<double> dStack;
    QStack<QString> aStack;
    QStack<QStringList> pStack;
    QStack<QVariant> stack;
    QMap<QString, pfunc> funcs;
    QMap<QString, QString> help;
    QMap<QString, QVariant> vars;
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
    int doList();
    int doClearProgram();
    int doRcl();
    int doSto();
    int doRun();
    int doDms();
    int doDdec();
    int doHms();
    int doHdec();
    int doADms();
    int doADdec();
    int doAHms();
    int doAHdec();
    int doD2r();
    int doR2d();
    int doShell();
    QString getNext();
    QString getPrevious();
    double fRand(double fMin, double fMax);
    QVariant ans;
    QVector <QString> entries;
    int iEntry;
    Constants *myConstants;
    int FixNumber;
    TypeStack getTypeAt(const QStack<QVariant> &s, AtStack i);
    bool isEnough(const QStack<QVariant> &s, int i);
    QString rawString(const QVariant v);

    static const int RC_OK;
    static const int RC_ANS;
    static const int RC_CLS;
    static const int RC_MODE;
    static const int RC_ALIAS;
    static const int RC_ALPHA;
    static const int RC_COMMAND;
    static const int RC_LOCK;
    static const int RC_EXIT;
    static const int RC_PROMPT;
    static const int RC_SHUTDOWN;
    static const int RC_ERR_UNKNOWN_COMMAND;
    static const int RC_ERR_TOO_FEW_ARGUMENTS;
    static const int RC_ERR_DIVISION_BY_ZERO;
    static const int RC_ERR_BAD_ARGUMENT;
    static const int RC_ERR_PROGRAM_STACK_EMPTY;
    static const QChar PREFIX_COMMAND;
    static const QChar PREFIX_ALIAS;
    static const QChar PREFIX_STORE;
    static const QChar PREFIX_FETCH;

private:
    double deg2rad(double);
    double rad2deg(double);
    std::mt19937_64 rng;
    std::uniform_real_distribution<double> unif;
};

#endif // RPN_H
