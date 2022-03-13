#include "rpn.h"

const int RPN::RC_OK = 0;
const int RPN::RC_ERR_UNKNOWN_COMMAND = 11;
const int RPN::RC_ANS = 12;
const int RPN::RC_ERR_TOO_FEW_ARGUMENTS = 13;
const int RPN::RC_ERR_DIVISION_BY_ZERO = 14;
const int RPN::RC_ERR_BAD_ARGUMENT = 15;
const int RPN::RC_LOCK = 16;
const int RPN::RC_EXIT = 17;
const int RPN::RC_SHUTDOWN = 18;
const int RPN::RC_ALIAS = 19;
const int RPN::RC_COMMAND = 20;
const int RPN::RC_CLS = 21;
const int RPN::RC_MODE = 22;
const int RPN::RC_ALPHA = 23;
const int RPN::RC_ERR_PROGRAM_STACK_EMPTY = 24;
const int RPN::RC_PROMPT = 101;
const QChar RPN::PREFIX_COMMAND = '$';
const QChar RPN::PREFIX_ALIAS = '@';
const QChar RPN::PREFIX_STORE = '#';
const QChar RPN::PREFIX_FETCH = '!';

/*
 *
 * 1 degree = pi/180 radians
 * 1 radian = 180/pi degrees
 *
 */


//******************************************************************************
// RPN()
//******************************************************************************
RPN::RPN(QMainWindow *mw) {
    this->mw = mw;
    this->iEntry = 0;
    this->entry = "";
    this->ans = 0;
    this->FixNumber = 6;
    initFuncs();
}

//******************************************************************************
// RPN()
//******************************************************************************
RPN::RPN(QMainWindow *mw, QString _entry) {
    this->mw = mw;
    this->iEntry = 0;
    this->entry = _entry.trimmed();
    this->ans = 0;
    this->entries.append(this->entry);
    this->FixNumber = 6;
    initFuncs();
}

//******************************************************************************
// setEntry()
//******************************************************************************
void RPN::setEntry(QString _entry) {
    this->entry = _entry.trimmed();
    this->entries.append(this->entry);
}

//******************************************************************************
// initFuncs()
//******************************************************************************
void RPN::initFuncs() {    
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    unif = std::uniform_real_distribution<double>(0, 1);

    myConstants = new Constants();
    funcs["+"] = &RPN::doPlus;
    funcs["-"] = &RPN::doMinus;
    funcs["*"] = &RPN::doMultiply;
    funcs["/"] = &RPN::doDivide;
    funcs["sin"] = &RPN::doSinus;
    funcs["cos"] = &RPN::doCosinus;
    funcs["tan"] = &RPN::doTangent;
    funcs["asin"] = &RPN::doArcSinus;
    funcs["acos"] = &RPN::doArcCosinus;
    funcs["atan"] = &RPN::doArcTangent;
    funcs["^"] = &RPN::doPower;
    funcs["sqr"] = &RPN::doSquare;
    funcs["sqrt"] = &RPN::doSquareRoot;
    funcs["inv"] = &RPN::doInverse;
    funcs["rnd"] = &RPN::doRandom;
    funcs["int"] = &RPN::doInteger;
    funcs["frac"] = &RPN::doFractional;
    funcs["pi"] = &RPN::doPI;
    funcs["deg"] = &RPN::doDegrees;
    funcs["rad"] = &RPN::doRadians;
    funcs["ln"] = &RPN::doLogNeper;
    funcs["exp"] = &RPN::doExponent;
    funcs["log"] = &RPN::doLog;
    funcs["10x"] = &RPN::doPower10;
    funcs["dup"] = &RPN::doDup;
    funcs["swap"] = &RPN::doSwap;
    funcs["drop"] = &RPN::doDrop;
    funcs["clear"] = &RPN::doClear;
    funcs["depth"] = &RPN::doDepth;
    funcs["ans"] = &RPN::doAns;
    funcs["last"] = &RPN::doAns;
    funcs["?"] = &RPN::doAns;
    funcs["abs"] = &RPN::doAbs;
    funcs["chs"] = &RPN::doChs;
    funcs["sign"] = &RPN::doSign;
    funcs["lock"] = &RPN::doLock;
    funcs["quit"] = &RPN::doQuit;
    funcs["exit"] = &RPN::doQuit;
    funcs["shutdown"] = &RPN::doShutdown;
    funcs["ver"] = &RPN::doVersion;
    funcs["version"] = &RPN::doVersion;
    funcs["time"] = &RPN::doTime;
    funcs["date"] = &RPN::doDate;
    funcs["day"] = &RPN::doDay;
    funcs["month"] = &RPN::doMonth;
    funcs["year"] = &RPN::doYear;
    funcs["hour"] = &RPN::doHour;
    funcs["minute"] = &RPN::doMinute;
    funcs["second"] = &RPN::doSecond;
    funcs["prompt"] = &RPN::doAPrompt;
    funcs["alen"] = &RPN::doALen;
    funcs["atime"] = &RPN::doATime;
    funcs["adate"] = &RPN::doADate;
    funcs["fix"] = &RPN::doFix;
    funcs["cls"] = &RPN::doCls;
    funcs["list"] = &RPN::doList;
    funcs["rcl"] = &RPN::doRcl;
    funcs["sto"] = &RPN::doSto;
    funcs["run"] = &RPN::doRun;
    funcs["shell"] = &RPN::doShell;
    /*
    funcs["dms"] = &RPN:: doDms;
    funcs["ddec"] = &RPN:: doDdec;
    funcs["hms"] = &RPN:: doHms;
    funcs["hdec"] = &RPN:: doHdec;
    funcs["adms"] = &RPN:: doADms;
    funcs["adec"] = &RPN:: doADdec;
    funcs["ahms"] = &RPN:: doAHms;
    funcs["ahdec"] = &RPN:: doAHdec;
    funcs["d2r"] = &RPN:: doD2r;
    funcs["r2d"] = &RPN:: doR2d;
    */

    help["+"] = "Adds or combines two expressions ( a1 a2 -- a3 )";
    help["-"] = "Subtracts two values ( a1 a2 -- a3 )";
    help["*"] = "Multiplies two values ( a1 a2 -- a3 )";
    help["/"] = "Divides two values ( a1 a2 -- a3 )";
    help["sin"] = "Returns the sine of the given angle (in radians) ( a1 -- a2 )";
    help["cos"] = "Returns the cosine of the given angle (in radians) ( a1 -- a2 )";
    help["tan"] = "Returns the tangent of the given angle (in radians) ( a1 -- a2 )";
    help["asin"] = "Returns the inverse sine (the arcsine) of a number ( a1 -- a2 )";
    help["acos"] = "Returns the inverse cosine (the arccosine) of a number ( a1 -- a2 )";
    help["atan"] = "Returns the inverse tangent (the arctangent) of a number ( a1 -- a2 )";
    help["^"] = "Returns a number raised to a power( a1 a2 -- a3 )";
    help["sqr"] = "Returns a number raised to a power of 2 ( a1 -- a2 )";
    help["sqrt"] = "Returns the positive square root of a number ( a1 -- a2 )";
    help["inv"] = "Returns 1 divided by the number on top of stack ( a1 -- a2 )";
    help["rnd"] = "Returns a random number between 0 and 1 ( -- a1 )";
    help["ln"] = "Returns the natural logarithm of a number ( a1 -- a2 )";
    help["exp"] = "Returns the base of the natural logarithm (e = 2.718282) raised to a power ( a1 -- a2 )";
    help["int"] = "Rounds a number down to the nearest integer ( a1 -- a2 )";
    help["frac"] = "Returns the fractional portion of a number ( a1 -- a2 )";
    help["abs"] = "Returns the absolute value of a numeric expression ( a1 -- a2 )";
    help["pi"] = "Push the PI value on the stack ( -- PI )";
    help["deg"] = "Set the angular mode as degrees ( -- )";
    help["rad"] = "Set the angular mode as radians ( -- )";
    help["log"] = "Returns the decimal logarithm of a number ( a1 -- a2 )";
    help["10x"] = "Returns 10 raised to a power ( a1 -- a2 )";
    help["dup"] = "Duplicates the value on the top of the stack ( a1 -- a1 a1 )";
    help["swap"] = "Exchanges the two values on the top of the stack ( a1 a2 -- a2 a1 )";
    help["drop"] = "Deletes the value on the top of the stack ( a1 -- )";
    help["clear"] = "Deletes all the values of the stack ( a1 an -- )";
    help["depth"] = "Returns the number of values of the stack ( -- a1 )";

    /*
    help["ans"] = &RPN::doAns;
    help["last"] = &RPN::doAns;
    help["?"] = &RPN::doAns;
    help["chs"] = &RPN::doChs;
    help["sign"] = &RPN::doSign;
    help["lock"] = &RPN::doLock;
    help["quit"] = &RPN::doQuit;
    help["exit"] = &RPN::doQuit;
    help["shutdown"] = &RPN::doShutdown;
    help["ver"] = &RPN::doVersion;
    help["version"] = &RPN::doVersion;
    help["time"] = &RPN::doTime;
    help["date"] = &RPN::doDate;
    help["day"] = &RPN::doDay;
    help["month"] = &RPN::doMonth;
    help["year"] = &RPN::doYear;
    help["hour"] = &RPN::doHour;
    help["minute"] = &RPN::doMinute;
    help["second"] = &RPN::doSecond;
    help["prompt"] = &RPN::doAPrompt;
    help["aprompt"] = &RPN::doAPrompt;
    help["adup"] = &RPN::doADup;
    help["adrop"] = &RPN::doADrop;
    help["aswap"] = &RPN::doASwap;
    help["aclear"] = &RPN::doAClear;
    help["alen"] = &RPN::doALen;
    help["adepth"] = &RPN::doADepth;
    help["atime"] = &RPN::doADepth;
    help["adate"] = &RPN::doADepth;
    help["fix"] = &RPN::doADepth;
    help["cls"] = &RPN::doCls;
    help["list"] = &RPN::doList;
    help["rcl"] = &RPN::doList;
    help["sto"] = &RPN::doList;
    help["run"] = &RPN::doRun;
    help["dms"] = &RPN:: doDms;
    help["ddec"] = &RPN:: doDdec;
    help["hms"] = &RPN:: doHms;
    help["hdec"] = &RPN:: doHdec;
    help["adms"] = &RPN:: doADms;
    help["adec"] = &RPN:: doADdec;
    help["ahms"] = &RPN:: doAHms;
    help["ahdec"] = &RPN:: doAHdec;
    help["d2r"] = &RPN:: doD2r;
    help["r2d"] = &RPN:: doR2d;
    help["shell"] = &RPN::doShell;
    */
}

//******************************************************************************
// doPlus()
//******************************************************************************
int RPN::doPlus(void) {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            double b = stack.pop().toDouble();
            double a = stack.pop().toDouble();
            stack.push(a + b);
        } else if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::STRING) {
            double a = stack.pop().toDouble();
            QString b = rawString(stack.pop());
            stack.push(b + QString::number(a));
        } else if (getTypeAt(stack, RPN::TOP_0) == RPN::STRING && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            QString a = rawString(stack.pop());
            double b = stack.pop().toDouble();
            stack.push(QString::number(b) + a);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doMinus()
//******************************************************************************
int RPN::doMinus(void) {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            double b = stack.pop().toDouble();
            double a = stack.pop().toDouble();
            stack.push(a - b);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doDivide()
//******************************************************************************
int RPN::doDivide() {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            double b = stack.pop().toDouble();
            double a = stack.pop().toDouble();
            if (b != 0) {
                stack.push(a / b);
            } else {
                RC = RC_ERR_DIVISION_BY_ZERO;
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doMultiply()
//******************************************************************************
int RPN::doMultiply() {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            double b = stack.pop().toDouble();
            double a = stack.pop().toDouble();
            stack.push(a * b);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doSinus()
//******************************************************************************
int RPN::doSinus() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(sin(M_PI * a / 180.0));
            } else {
                stack.push(sin(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doCosinus()
//******************************************************************************
int RPN::doCosinus() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(cos(M_PI * a / 180.0));
            } else {
                stack.push(cos(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doTangent()
//******************************************************************************
int RPN::doTangent() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(tan(M_PI * a / 180.0));
            } else {
                stack.push(tan(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doArcSinus()
//******************************************************************************
int RPN::doArcSinus() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(asin(a) * 180.0 / M_PI);
            } else {
                stack.push(asin(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doArcCosinus()
//******************************************************************************
int RPN::doArcCosinus() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(acos(a) * 180.0 / M_PI);
            } else {
                stack.push(acos(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doArcTangent()
//******************************************************************************
int RPN::doArcTangent() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (angularMode == DEGREES) {
                stack.push(atan(a) * 180.0 / M_PI);
            } else {
                stack.push(atan(a));
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doPower()
//******************************************************************************
int RPN::doPower() {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER && getTypeAt(stack, RPN::TOP_1) == RPN::NUMBER) {
            double b = stack.pop().toDouble();
            double a = stack.pop().toDouble();
            stack.push(pow(a, b));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doSquare()
//******************************************************************************
int RPN::doSquare() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(a * a);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doSquareRoot()
//******************************************************************************
int RPN::doSquareRoot() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(sqrt(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doInverse()
//******************************************************************************
int RPN::doInverse() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (a != 0) {
                stack.push(1 / a);
            } else {
                RC = RC_ERR_DIVISION_BY_ZERO;
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doRandom()
//******************************************************************************
int RPN::doRandom() {
    int RC = RC_OK;
    stack.push(fRand(0, 1));
    return RC;
}

//******************************************************************************
// doInteger()
//******************************************************************************
int RPN::doInteger() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(floor(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doFractional()
//******************************************************************************
int RPN::doFractional() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(a - floor(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doPI()
//******************************************************************************
int RPN::doPI() {
    int RC = RC_OK;
    stack.push(M_PI);
    return RC;
}

//******************************************************************************
// doDegrees()
//******************************************************************************
int RPN::doDegrees() {
    int RC = RC_MODE;
    this->angularMode = RPN::DEGREES;
    return RC;
}

//******************************************************************************
// doRadians()
//******************************************************************************
int RPN::doRadians() {
    int RC = RC_MODE;
    this->angularMode = RPN::RADIANS;
    return RC;
}

//******************************************************************************
// doLogNeper()
//******************************************************************************
int RPN::doLogNeper() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(log(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doExponent()
//******************************************************************************
int RPN::doExponent() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(exp(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doLog()
//******************************************************************************
int RPN::doLog() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(log10(a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doPower10()
//******************************************************************************
int RPN::doPower10() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(pow(10.0, a));
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doDup()
//******************************************************************************
int RPN::doDup() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(a);
            stack.push(a);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;}

//******************************************************************************
// doSwap()
//******************************************************************************
int RPN::doSwap() {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        QVariant b = stack.pop();
        QVariant a = stack.pop();
        stack.push(b);
        stack.push(a);
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doDrop()
//******************************************************************************
int RPN::doDrop() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        stack.pop();
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doFix()
//******************************************************************************
int RPN::doFix() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            this->FixNumber = a;
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doClear()
//******************************************************************************
int RPN::doClear() {
    int RC = RC_OK;
    stack.clear();
    return RC;
}

//******************************************************************************
// doDepth()
//******************************************************************************
int RPN::doDepth() {
    int RC = RC_OK;
    stack.push(stack.length());
    return RC;
}

//******************************************************************************
// doAns()
//******************************************************************************
int RPN::doAns() {
    int RC = RC_ANS;
    return RC;
}

//******************************************************************************
// doCls()
//******************************************************************************
int RPN::doCls() {
    int RC = RC_CLS;
    return RC;
}

//******************************************************************************
// doAbs()
//******************************************************************************
int RPN::doAbs() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (a >= 0.0) {
                stack.push(a);
            } else {
                stack.push(-1.0 * a);
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doChs()
//******************************************************************************
int RPN::doChs() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            stack.push(-1.0 * a);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doSign()
//******************************************************************************
int RPN::doSign() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NUMBER) {
            double a = stack.pop().toDouble();
            if (a > 0.0) {
                stack.push(1.0);
            } else if (a < 0.0) {
                stack.push(-1.0);
            } else {
                stack.push(0.0);
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doList()
//******************************************************************************
int RPN:: doList() {
    int RC= RC_ALPHA;
    if (pStack.length() >= 1) {
        this->alpha = "PROGRAM STACK :\n";
        int i(0);
        for (QStringList l : pStack) {
            this->alpha = this->alpha + QString::number(i) + ": { " + l.join(" ") + " }\n";
            i++;
        }
    } else {
        RC = RC_ERR_PROGRAM_STACK_EMPTY;
    }
    return RC;
}

//******************************************************************************
// fRand()
//******************************************************************************
double RPN::fRand(double fMin, double fMax) {
    double f = unif(rng);
    return fMin + f * (fMax - fMin);
}

//******************************************************************************
// doShell()
//******************************************************************************
int RPN::doShell() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::STRING) {
            out = "$" + rawString(stack.pop().toString());
            RC = RC_COMMAND;
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// xeq()
//******************************************************************************
int RPN::xeq() {
    int RC = RC_ERR_UNKNOWN_COMMAND;
    if (entry.at(0) == RPN::PREFIX_COMMAND) {
        out = entry;
        return RC_COMMAND;
    }

    if (entry.at(0) == RPN::PREFIX_ALIAS) {
        // TODO : Execute alias
        return RC_ALIAS;
    }

    QStringList tokens;
    QStringList tmpList = entry.split(QRegExp("\""));
    QStringList program;
    bool inString(false);
    bool inCode(false);
    tokens.clear();
    foreach (QString s, tmpList) {
        if (inString) {
            tokens.append("\""+ s + "\"");
        } else {
            tokens.append(s.split(QRegExp("\\s+")));
        }
        inString = !inString;
    }

    for(auto& token : tokens) {
        if (token.length() > 0) {
            if (token == "{") {
                program.clear();
                inCode = true;
            } else if (token == "}") {
                inCode = false;
                // Do something with the stored code...
                qDebug().noquote() << program;
                stack.push(program);
                RC = RC_OK;
            } else {
                qDebug().noquote() << token;
                if (token.at(0) == "\"") {                  // STRING
                    if (token.at(token.length()-1) == "\"") {
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            stack.push(token);
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                        }
                    } else {
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            stack.push(token + "\"");
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                        }
                    }
                    RC = RC_OK;
                } else if (token.at(0) == "'") {            // NAME
                    if (token.at(token.length()-1) == "'") {
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            stack.push(token);
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                        }
                    } else {
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            stack.push(token + "'");
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                        }
                    }
                    RC = RC_OK;
                } else if (token.at(0) == RPN::PREFIX_STORE) {

                } else if (token.at(0) == RPN::PREFIX_FETCH) {

                } else {
                    bool ok(false);
                    double d = token.toDouble(&ok);
                    if (ok) {
                        RC = RC_OK;
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            stack.push(d);
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                        }
                    } else {
                        if (!inCode) {
                            qDebug("NOT INCODE");
                            if (funcs.contains(token)) {
                                RC = (this->*funcs[token])();
                            }
                        } else {
                            qDebug("INCODE");
                            program.append(token);
                            RC = RC_OK;
                        }
                    }
                }
            }
        }
    }
    if (RC == RC_OK) {
        if (!stack.isEmpty()) {
            if (getTypeAt(stack, TOP_0) == RPN::CODE) {
                ans = "{ " + stack.top().toStringList().join(" ") + " }";
            } else {
                ans = stack.top();
            }
            out = ans.toString();
        } else {
            out = "EMPTY STACK";
        }
    } else if (RC == RC_ANS) {
        RC = RC_OK;
        out = ans.toString();
    } else if (RC == RC_CLS) {
        RC = RC_CLS;
    } else if (RC == RC_PROMPT) {
        out = rawString(stack.pop());
    } else {
        switch (RC) {
            case RC_ERR_UNKNOWN_COMMAND:
            error = "UNKNOWN COMMAND";
            break;

            case RC_ERR_TOO_FEW_ARGUMENTS:
            error = "TOO FEW ARGUMENTS";
            break;

            case RC_ERR_DIVISION_BY_ZERO:
            error = "DIVISION BY ZERO";
            break;

            case RC_ERR_BAD_ARGUMENT:
            error = "BAD ARGUMENT";
            break;

            default:
            error = "RPN ERROR";
            break;
        }
    }
    qDebug() << stack;
    return RC;
}

//******************************************************************************
// getNext()
//******************************************************************************
QString RPN::getNext() {
    if (!entries.isEmpty()) {
        iEntry++;
        if (iEntry >= entries.length()) {
            iEntry = 0;
        }
        return entries[iEntry];
    } else {
        return "";
    }
}

//******************************************************************************
// getPrevious()
//******************************************************************************
QString RPN::getPrevious() {
    if (!entries.isEmpty()) {
        iEntry--;
        if (iEntry < 0) {
            iEntry = entries.length() - 1;
        }
        return entries[iEntry];
    } else {
        return "";
    }
}

//******************************************************************************
// doVersion()
//******************************************************************************
int RPN::doVersion() {
    int RC = RC_OK;
    stack.push(myConstants->getInt("NVERSION"));
    return RC;
}

//******************************************************************************
// doTime()
//******************************************************************************
int RPN::doTime() {
    int RC = RC_OK;
    int h = QTime::currentTime().hour();
    int m = QTime::currentTime().minute();
    int s = QTime::currentTime().second();
    stack.push(h + m / 100.0 + s / 10000.0);
    return RC;
}

//******************************************************************************
// doDay()
//******************************************************************************
int RPN::doDay() {
    int RC = RC_OK;
    stack.push(QDate::currentDate().day());
    return RC;
}

//******************************************************************************
// doMonth()
//******************************************************************************
int RPN::doMonth() {
    int RC = RC_OK;
    stack.push(QDate::currentDate().month());
    return RC;
}

//******************************************************************************
// doYear()
//******************************************************************************
int RPN::doYear() {
    int RC = RC_OK;
    stack.push(QDate::currentDate().year());
    return RC;
}

//******************************************************************************
// doDate()
//******************************************************************************
int RPN::doDate() {
    int RC = RC_OK;
    int y = QDate::currentDate().year();
    int m = QDate::currentDate().month();
    int d = QDate::currentDate().day();
    stack.push(y + m / 100.0 + d / 10000.0);
    return RC;
}

//******************************************************************************
// doHour()
//******************************************************************************
int RPN::doHour() {
    int RC = RC_OK;
    stack.push(QTime::currentTime().hour());
    return RC;
}

//******************************************************************************
// doMinute()
//******************************************************************************
int RPN::doMinute() {
    int RC = RC_OK;
    stack.push(QTime::currentTime().minute());
    return RC;
}

//******************************************************************************
// doSecond()
//******************************************************************************
int RPN::doSecond() {
    int RC = RC_OK;
    stack.push(QTime::currentTime().second());
    return RC;
}

//******************************************************************************
// doLock()
//******************************************************************************
int RPN::doLock() {
    int RC = RC_LOCK;
    return RC;
}

//******************************************************************************
// doQuit()
//******************************************************************************
int RPN::doQuit() {
    int RC = RC_EXIT;
    return RC;
}

//******************************************************************************
// doShutdown()
//******************************************************************************
int RPN::doShutdown() {
    int RC = RC_SHUTDOWN;
    return RC;
}

//******************************************************************************
// doAPrompt()
//******************************************************************************
int RPN::doAPrompt() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::STRING) {
            RC = RC_PROMPT;
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doALen()
//******************************************************************************
int RPN::doALen() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::STRING) {
            stack.push(rawString(stack.top().toString()).length());
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doATime()
//******************************************************************************
int RPN::doATime() {
    int RC = RC_OK;
    QString t = QDateTime::currentDateTime().toString("HH:mm:ss");
    stack.push("\""+ t + "\"");
    return RC;
}

//******************************************************************************
// doADate()
//******************************************************************************
int RPN::doADate() {
    int RC = RC_OK;
    QString d = QDateTime::currentDateTime().toString("dd/MM/yyyy");
    stack.push("\"" + d + "\"");
    return RC;
}

//******************************************************************************
// doRcl()
//******************************************************************************
int RPN::doRcl() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NAME) {
            QString name = rawString(stack.pop());
            stack.push(vars[name]);
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doRcl()
//******************************************************************************
int RPN::doSto() {
    int RC = RC_OK;
    if (isEnough(stack, 2)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NAME) {
            QString name = rawString(stack.pop());
            QVariant a = stack.pop();
            vars[name] = a;
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doRun()
//******************************************************************************
int RPN::doRun() {
    int RC = RC_OK;
    if (isEnough(stack, 1)) {
        if (getTypeAt(stack, RPN::TOP_0) == RPN::NAME) {
            QString name = rawString(stack.pop());
            stack.push(vars[name]);
            if (getTypeAt(stack, RPN::TOP_0) == RPN::CODE) {
                QStringList pgm = stack.pop().toStringList();
                for (QString inst : pgm) {
                    qDebug() << "RUN_PGM";
                    setEntry(inst);
                    xeq();
                    // lw->displayOutput(inst, out);
                    /*
                    int rc = rpn->xeq();
                    if (rc == RPN::RC_OK) {
                        displayOutput(cmd, rpn->out);
                    } else if (rc == RPN::RC_CLS) {
                        mMainWindow->ui->txtConsole->setText("");
                        showMessage("Console cleared");
                    } else if (rc == RPN::RC_MODE) {
                        lblAngularMode->setText((rpn->angularMode==RPN::DEGREES) ? "DEG" : "RAD");
                        displayOutput(cmd, rpn->out);
                        // TODO : Display stack
                    } else if (rc == RPN::RC_LOCK) {
                        slotLock();
                    } else if (rc == RPN::RC_EXIT) {
                        slotClosed();
                    } else if (rc == RPN::RC_SHUTDOWN) {
                        slotOff();
                    } else if (rc == RPN::RC_ALPHA) {
                        displayOutput(cmd, rpn->alpha);
                    } else if (rc == RPN::RC_ALIAS) {
                        // TODO : Display ALIAS result
                    } else if (rc == RPN::RC_COMMAND) {
                        runCommand("./", rpn->out.mid(1), mMainWindow->ui->txtConsole);
                    } else if (rc == RPN::RC_PROMPT) {
                        lblPrompt->setText(rpn->out + " : ");
                        displayStack(rpn->stack);
                        displayVars(rpn->vars);
                        lblDepthStack->setText(QString::number(rpn->stack.length()));
                    } else {
                        displayOutput(cmd, rpn->error);
                    }
                    */
                }
            } else {
                stack.pop();
            }
        } else {
            RC = RC_ERR_BAD_ARGUMENT;
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doScratchName()
//******************************************************************************
int RPN::doScratchName() {
    int RC = RC_OK;
    QString adjectives[] = {
        "tiny",
        "big",
        "shiny",
        "fast",
        "furious",
        "slow",
        "gentle",
        "slim",
        "tall",
        "thin",
        "short",
        "sharp",
        "happy",
        "bored",
        "lucky",
        "strong",
        "weak",
        "mad",
        "tired",
        "clever",
        "smart",
        "light",
        "heavy",
        "foolish",
        "black",
        "white",
        "blue",
        "green",
        "red",
        "yellow",
        "brown",
        "pink",
        "purple",
        "orange",
        "silver",
        "gold",
        "noisy",
        "freaky",
        "little",
        "hairy",
        "bold",
        "brave",
        "fat",
        "rude",
        "pretty",
        "shy",
        "grumpy",
        "sad",
        "tedious",
        "cold",
        "hot"
    };

    QString animals[] = {
        "dog",
        "cat",
        "bee",
        "wasp",
        "hornet",
        "fish",
        "cow",
        "puppy",
        "bird",
        "parrot",
        "canary",
        "turtle",
        "rabbit",
        "mouse",
        "snake",
        "lizard",
        "pony",
        "horse",
        "pig",
        "hen",
        "rooster",
        "goose",
        "turkey",
        "duck",
        "sheep",
        "ram",
        "lamb",
        "goat",
        "donkey",
        "bull",
        "ox",
        "calf",
        "hare",
        "beetle",
        "snail",
        "slug",
        "ant",
        "spider",
        "worm",
        "dove",
        "crow",
        "swan",
        "fox",
        "owl",
        "beaver",
        "bat",
        "weasel",
        "deer",
        "doe",
        "tuna",
        "salmon",
        "lobster",
        "shrimp",
        "whale",
        "shark",
        "dolphin",
        "walrus",
        "seal",
        "otter",
        "panda",
        "tiger",
        "lion",
        "bear",
        "frog",
        "toad"
    };

    QString d = QDateTime::currentDateTime().toString("dd/MM/yyyy");
    aStack.push(d);
    return RC;
}

//******************************************************************************
// deg2rad()
//******************************************************************************
double RPN::deg2rad(double a) {
    return (a * 180.0 / M_PI);
}

//******************************************************************************
// rad2deg()
//******************************************************************************
double RPN::rad2deg(double a) {
    return (a * M_PI / 180.0);
}

//******************************************************************************
// isEnough()
//******************************************************************************
bool RPN::isEnough(const QStack<QVariant> &s, int i) {
    return (s.length() >= i);
}

//******************************************************************************
// rawString()
//******************************************************************************
QString RPN::rawString(const QVariant v) {
    QString s = v.toString();
    if (s.at(0) == "\"" || s.at(0) == "'") {
        s = s.mid(1);
    }
    if (s.at(s.length() - 1) == "\"" || s.at(s.length() - 1) == "'") {
        s = s.mid(0, s.length() - 1);
    }
    return s;
}

//******************************************************************************
// getTypeAt()
//******************************************************************************
RPN::TypeStack RPN::getTypeAt(const QStack<QVariant> &s, AtStack i) {
    int TOP = s.length() - 1;
    QVariant item;
    switch (i) {
    case TOP_0:
        item = s.at(TOP);
        break;
    case TOP_1:
        item = s.at(TOP - 1);
        break;
    case TOP_2:
        item = s.at(TOP - 2);
        break;
    }

    switch (item.userType()) {
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
        case QMetaType::Double:
        case QMetaType::Float:
            return RPN::NUMBER;

        case QMetaType::QString:
            if (item.toString().at(0) == "\"")
                return RPN::STRING;
            else
                return RPN::NAME;

        case QMetaType::QStringList:
            return RPN::CODE;

        default:
            return RPN::STRING;
    }
}
