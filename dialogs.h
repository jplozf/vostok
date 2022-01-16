#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QTextCharFormat>
#include <QGraphicsTextItem>
#include <QSpinBox>
#include <QTimeEdit>
#include <QDirIterator>
#include <QComboBox>
#include <QSound>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "shortcuts.h"
#include "settings.h"
#include "alarms.h"

//******************************************************************************
// NewShortcutDialog()
//******************************************************************************
class NewShortcutDialog : public QDialog
{
    Q_OBJECT

public:
    NewShortcutDialog(QWidget*);
    QString getShortcutName();
    QString getShortcutCommand();
    QString getShortcutIcon();
    QString getShortcutComment();
    QString getShortcutType();

private:
    QLabel *lblType;
    QComboBox *cbxType;
    QLabel *lblName;
    QLineEdit *txtName;
    QLabel *lblCommand;
    QLineEdit *txtCommand;
    QPushButton *btnBrowse;
    QLabel *lblComment;
    QLineEdit *txtComment;
    QPushButton *btnIcon;
    QString strIcon;
    QPushButton *btnOK;
    QPushButton *btnCancel;
    void done(int);

private slots:
    void browseCommand();
    void browseIcon();

};

//******************************************************************************
// EditShortcutDialog()
//******************************************************************************
class EditShortcutDialog : public QDialog
{
    Q_OBJECT

public:
    EditShortcutDialog(QWidget*, Shortcut*);
    QString getShortcutName();
    QString getShortcutCommand();
    QString getShortcutIcon();
    QString getShortcutComment();
    QString getShortcutType();

private:
    QLabel *lblType;
    QComboBox *cbxType;
    QLabel *lblName;
    QLineEdit *txtName;
    QLabel *lblCommand;
    QLineEdit *txtCommand;
    QPushButton *btnBrowse;
    QLabel *lblComment;
    QLineEdit *txtComment;
    QPushButton *btnIcon;
    QString strIcon;
    QPushButton *btnOK;
    QPushButton *btnCancel;
    void done(int);

private slots:
    void browseCommand();
    void browseIcon();

};

//******************************************************************************
// NewFolderDialog()
//******************************************************************************
class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    NewFolderDialog(QWidget*);
    QString getFolderName();

private:
    QLabel *lblName;
    QLineEdit *txtName;
    QLabel *lblComment;
    QLineEdit *txtComment;
    QPushButton *btnOK;
    QPushButton *btnCancel;
};

//******************************************************************************
// OSDDialog()
//******************************************************************************
class OSDDialog : public QDialog {
    Q_OBJECT

public:
    OSDDialog(QWidget*, QString*);
    ~OSDDialog();

private:
    QTimer *tmrDisplay;
};

//******************************************************************************
// OSDLabel()
//******************************************************************************
class OSDLabel : public QLabel
{
    Q_OBJECT
public:
    explicit OSDLabel(QWidget *parent = nullptr);
    explicit OSDLabel(QString *text);

protected:
    void paintEvent(QPaintEvent *) override;

signals:

};


//******************************************************************************
// PowerDialog()
//******************************************************************************
class PowerDialog : public QDialog {
    Q_OBJECT

public:
    PowerDialog(QWidget*);
    int getTimeout();
    static const int CANCEL;
    static const int LOCK;
    static const int SHUTDOWN;
    static const int REBOOT;
    static const int EXIT;

private:
    void done(int rc);
    QPushButton *btnCancel;
    QPushButton *btnLock;
    QPushButton *btnShutdown;
    QPushButton *btnReboot;
    QPushButton *btnExit;
    QSpinBox *spnTimeout;

private slots:
    void returnLock();
    void returnShutdown();
    void returnReboot();
    void returnExit();
    void returnCancel();
};

//******************************************************************************
// AlarmsDialog()
//******************************************************************************
class AlarmsDialog : public QDialog {
    Q_OBJECT

public:
    AlarmsDialog(QWidget*);
    Alarms::alarm a;
    QTableWidget *tblAlarms;

private:
    QCheckBox *chkEnabled;
    QTimeEdit *tmeAlarm;
    QCheckBox *chkMonday;
    QCheckBox *chkTuesday;
    QCheckBox *chkWednesday;
    QCheckBox *chkThursday;
    QCheckBox *chkFriday;
    QCheckBox *chkSaturday;
    QCheckBox *chkSunday;
    QLineEdit *txtMessage;
    QPushButton *btnPlay;
    QComboBox *lstSounds;
    QPushButton *btnCancel;
    QPushButton *btnOK;

private slots:
    void playSound();
    void done(int rc);
};

#endif // DIALOGS_H
