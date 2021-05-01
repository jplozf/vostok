#include "interpreter.h"

const int Interpreter::RC_OK = 0;
const int Interpreter::RC_OFF = 255;
const int Interpreter::RC_EXIT = 27;

//******************************************************************************
// Interpreter()
//******************************************************************************
Interpreter::Interpreter()
{
    this->command = "";
}

//******************************************************************************
// Interpreter()
//******************************************************************************
Interpreter::Interpreter(QString _command) {
    this->command = _command;
}

//******************************************************************************
// setCommand()
//******************************************************************************
void Interpreter::setCommand(QString _command) {
    this->command = _command;
}

//******************************************************************************
// getRC()
//******************************************************************************
int Interpreter::getRC() {
    return this->rc;
}

//******************************************************************************
// xeq()
//******************************************************************************
QString Interpreter::xeq(QString _command) {
    this->command = _command;
    return xeq();
}

//******************************************************************************
// xeq()
//******************************************************************************
QString Interpreter::xeq() {
    Settings *mySettings = new Settings();
    Constants * myConstants = new Constants();
    QString out = "0/0";
    rc = this->RC_OK;
    tokens = this->command.split(QRegExp("\\s+"));

    QString cmd = pop();
    if (cmd.toLower() == "version") {
        //**********************************************************************
        // VERSION
        //**********************************************************************
        out = "Version " + myConstants->getQString("VERSION");
    } else if (cmd.toLower() == "dir") {
        //**********************************************************************
        // DIR
        //**********************************************************************
        QString loc = pop();
        if (loc.isNull()) {
            loc = "./";
        }
        QDir dir(loc);
        QStringList files = dir.entryList();
        out = "";
        for (const auto& file : files) {
            out = out + file + "\n";
        }
    } else if (cmd.toLower() == "time") {
        //**********************************************************************
        // TIME
        //**********************************************************************
        QString time1 = QDateTime::currentDateTime().toString(mySettings->get("TITLE_BAR_TIME_FORMAT").toString());
        out = time1;
    }  else if (cmd.toLower() == "lock") {
        //**********************************************************************
        // LOCK
        //**********************************************************************
        QProcess *process = new QProcess();
#ifdef Q_OS_LINUX
        QString program = "dbus-send --type=method_call --dest=org.mate.ScreenSaver /org/mate/ScreenSaver org.mate.ScreenSaver.Lock";
        process->start(program);
#endif
#ifdef Q_OS_WIN32
        QString program = "rundll32.exe user32.dll,LockWorkStation";
        process->start(program);
#endif
    }  else if (cmd.toLower() == "exit") {
        //**********************************************************************
        // EXIT
        //**********************************************************************
        out = "Exiting";
        rc = this->RC_EXIT;
    } else {
        out = "Error : Can't " + cmd;
    }
    tokens.clear();
    delete(myConstants);
    delete(mySettings);
    return out;
}

//******************************************************************************
// pop()
//******************************************************************************
QString Interpreter::pop() {
    QString rc = nullptr;
    if (tokens.size() != 0) {
        rc = tokens[0];
        tokens.removeAt(0);
    }
    return rc;
}
