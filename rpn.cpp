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
const int RPN::RC_PROMPT = 101;
const QChar RPN::PREFIX_COMMAND = '$';
const QChar RPN::PREFIX_ALIAS = '!';


//******************************************************************************
// RPN()
//******************************************************************************
RPN::RPN() {
    this->iEntry = 0;
    this->entry = "";
    this->ans = 0;
    this->FixNumber = 6;
    initFuncs();
}

//******************************************************************************
// RPN()
//******************************************************************************
RPN::RPN(QString _entry) {
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
    funcs["aprompt"] = &RPN::doAPrompt;
    funcs["adup"] = &RPN::doADup;
    funcs["adrop"] = &RPN::doADrop;
    funcs["aswap"] = &RPN::doASwap;
    funcs["aclear"] = &RPN::doAClear;
    funcs["alen"] = &RPN::doALen;
    funcs["adepth"] = &RPN::doADepth;
    funcs["atime"] = &RPN::doATime;
    funcs["adate"] = &RPN::doADate;
    funcs["fix"] = &RPN::doFix;
    funcs["cls"] = &RPN::doCls;

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

    /*
    help["pi"] = &RPN::doPI;
    help["deg"] = &RPN::doDegrees;
    help["rad"] = &RPN::doRadians;
    help["log"] = &RPN::doLog;
    help["10x"] = &RPN::doPower10;
    help["dup"] = &RPN::doDup;
    help["swap"] = &RPN::doSwap;
    help["drop"] = &RPN::doDrop;
    help["clear"] = &RPN::doClear;
    help["depth"] = &RPN::doDepth;
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
    */
}

//******************************************************************************
// doPlus()
//******************************************************************************
int RPN::doPlus(void) {
    int RC = RC_OK;
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        dStack.push(a + b);
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
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        dStack.push(a - b);
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
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        if (b != 0) {
            dStack.push(a / b);
        } else {
            RC = RC_ERR_DIVISION_BY_ZERO;
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
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        dStack.push(a * b);
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(sin(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(cos(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(tan(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(asin(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(acos(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(atan(a));
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
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        if (b != 0) {
            dStack.push(pow(a, b));
        } else {
            RC = RC_ERR_DIVISION_BY_ZERO;
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(a * a);
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(sqrt(a));
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        if (a != 0) {
            dStack.push(1 / a);
        } else {
            RC = RC_ERR_DIVISION_BY_ZERO;
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
    dStack.push(fRand(0, 1));
    return RC;
}

//******************************************************************************
// doInteger()
//******************************************************************************
int RPN::doInteger() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(floor(a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doFractional()
//******************************************************************************
int RPN::doFractional() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(a - floor(a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doPI()
//******************************************************************************
int RPN::doPI() {
    int RC = RC_OK;
    dStack.push(M_PI);
    return RC;
}

//******************************************************************************
// doDegrees()
//******************************************************************************
int RPN::doDegrees() {
    int RC = RC_OK;
    return RC;

}

//******************************************************************************
// doRadians()
//******************************************************************************
int RPN::doRadians() {
    int RC = RC_OK;
    return RC;
}

//******************************************************************************
// doLogNeper()
//******************************************************************************
int RPN::doLogNeper() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(log(a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doExponent()
//******************************************************************************
int RPN::doExponent() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(exp(a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doLog()
//******************************************************************************
int RPN::doLog() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(log10(a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doPower10()
//******************************************************************************
int RPN::doPower10() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(pow(10.0, a));
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doDup()
//******************************************************************************
int RPN::doDup() {
    int RC = RC_OK;
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(a);
        dStack.push(a);
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doSwap()
//******************************************************************************
int RPN::doSwap() {
    int RC = RC_OK;
    if (dStack.length() >= 2) {
        double b = dStack.pop();
        double a = dStack.pop();
        dStack.push(b);
        dStack.push(a);
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
    if (dStack.length() >= 1) {
        dStack.pop();
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
    if (dStack.length() >= 1) {
        this->FixNumber = dStack.pop();
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
    dStack.clear();
    return RC;
}

//******************************************************************************
// doDepth()
//******************************************************************************
int RPN::doDepth() {
    int RC = RC_OK;
    dStack.push(dStack.length());
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        if (a >= 0.0) {
            dStack.push(a);
        } else {
            dStack.push(-1.0 * a);
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        dStack.push(-1.0 * a);
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
    if (dStack.length() >= 1) {
        double a = dStack.pop();
        if (a > 0.0) {
            dStack.push(1.0);
        } else if (a < 0.0) {
            dStack.push(-1.0);
        } else {
            dStack.push(0.0);
        }
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// fRand()
//******************************************************************************
double RPN::fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

//******************************************************************************
// xeq()
//******************************************************************************
int RPN::xeq() {
    int RC = RC_ERR_UNKNOWN_COMMAND;
    // std::setprecision(std::numeric_limits<double>::digits10 + 1);
    // std::setprecision(9);
    // std::cout.setf(std::ios::fixed, std::ios::floatfield);
    // std::cout.unsetf (std::ios::floatfield);
    // std::cout.precision(10);

    if (entry.at(0) == RPN::PREFIX_COMMAND) {
        // TODO : Execute command
        return RC_COMMAND;
    }

    if (entry.at(0) == RPN::PREFIX_ALIAS) {
        // TODO : Execute alias
        return RC_ALIAS;
    }

    QStringList tokens;
    QStringList tmpList = entry.split(QRegExp("\""));
    bool inside = false;
    tokens.clear();
    foreach (QString s, tmpList) {
        if (inside) {
            tokens.append("\""+ s + "\"");
        } else {
            tokens.append(s.split(QRegExp("\\s+")));
        }
        inside = !inside;
    }

    for(auto& token : tokens) {
        if (token.length() > 0) {
            qDebug().noquote() << token;
            if (token.at(0) == "\"") {
                if (token.at(token.length()-1) == "\"") {
                    aStack.push(token.mid(1, token.length() - 2));
                } else {
                    aStack.push(token.mid(1, token.length() - 1));
                }
                RC = RC_OK;
            } else {
                bool ok(false);
                double d = token.toDouble(&ok);
                if (ok) {
                    RC = RC_OK;
                    dStack.push(d);
                    // qDebug("%f", d);
                } else {
                    if (funcs.contains(token)) {
                        RC = (this->*funcs[token])();
                    }
                }
            }
        }
    }
    if (RC == RC_OK) {
        if (!dStack.isEmpty()) {
            ans = dStack.top();
            out = QString::number(ans, 'G', this->FixNumber);
        } else {
            out = "EMPTY STACK";
        }
    } else if (RC == RC_ANS) {
        RC = RC_OK;
        out = QString::number(ans, 'G', this->FixNumber);
    } else if (RC == RC_CLS) {
        RC = RC_CLS;
    } else if (RC == RC_PROMPT) {
        RC = RC_OK;
        out = aStack.top();
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
    dStack.push(myConstants->getInt("NVERSION"));
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
    dStack.push(h + m / 100.0 + s / 10000.0);
    return RC;
}

//******************************************************************************
// doDay()
//******************************************************************************
int RPN::doDay() {
    int RC = RC_OK;
    dStack.push(QDate::currentDate().day());
    return RC;
}

//******************************************************************************
// doMonth()
//******************************************************************************
int RPN::doMonth() {
    int RC = RC_OK;
    dStack.push(QDate::currentDate().month());
    return RC;
}

//******************************************************************************
// doYear()
//******************************************************************************
int RPN::doYear() {
    int RC = RC_OK;
    dStack.push(QDate::currentDate().year());
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
    dStack.push(y + m / 100.0 + d / 10000.0);
    return RC;
}

//******************************************************************************
// doHour()
//******************************************************************************
int RPN::doHour() {
    int RC = RC_OK;
    dStack.push(QTime::currentTime().hour());
    return RC;
}

//******************************************************************************
// doMinute()
//******************************************************************************
int RPN::doMinute() {
    int RC = RC_OK;
    dStack.push(QTime::currentTime().minute());
    return RC;
}

//******************************************************************************
// doSecond()
//******************************************************************************
int RPN::doSecond() {
    int RC = RC_OK;
    dStack.push(QTime::currentTime().second());
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
    if (aStack.length() >= 1) {
        RC = RC_PROMPT;
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doADrop()
//******************************************************************************
int RPN::doADrop() {
    int RC = RC_OK;
    if (aStack.length() >= 1) {
        aStack.pop();
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }    return RC;
}

//******************************************************************************
// doADup()
//******************************************************************************
int RPN::doADup() {
    int RC = RC_OK;
    if (aStack.length() >= 1) {
        QString a = aStack.pop();
        aStack.push(a);
        aStack.push(a);
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }    return RC;
}

//******************************************************************************
// doASwap()
//******************************************************************************
int RPN::doASwap() {
    int RC = RC_OK;
    if (aStack.length() >= 2) {
        QString b = aStack.pop();
        QString a = aStack.pop();
        aStack.push(b);
        aStack.push(a);
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }    return RC;
}

//******************************************************************************
// doAClear()
//******************************************************************************
int RPN::doAClear() {
    int RC = RC_OK;
    aStack.clear();
    return RC;
}

//******************************************************************************
// doALen()
//******************************************************************************
int RPN::doALen() {
    int RC = RC_OK;
    if (aStack.length() >= 1) {
        dStack.push(aStack.top().length());
    } else {
        RC = RC_ERR_TOO_FEW_ARGUMENTS;
    }
    return RC;
}

//******************************************************************************
// doADepth()
//******************************************************************************
int RPN::doADepth() {
    int RC = RC_OK;
    dStack.push(aStack.length());
    return RC;
}

//******************************************************************************
// doATime()
//******************************************************************************
int RPN::doATime() {
    int RC = RC_OK;
    QString t = QDateTime::currentDateTime().toString("HH:mm:ss");
    aStack.push(t);
    return RC;
}

//******************************************************************************
// doADate()
//******************************************************************************
int RPN::doADate() {
    int RC = RC_OK;
    QString d = QDateTime::currentDateTime().toString("dd/MM/yyyy");
    aStack.push(d);
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
