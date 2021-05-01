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
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include "shortcuts.h"
#include "settings.h"

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


#endif // DIALOGS_H
