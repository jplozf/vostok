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

class NewShortcutDialog : public QDialog
{
public:
    NewShortcutDialog(QWidget*);
    QString getShortcutName();
    QString getShortcutCommand();
    QString getShortcutArgs();

private:
    QLabel *lblName;
    QLineEdit *txtName;
    QLabel *lblCommand;
    QLineEdit *txtCommand;
    QPushButton *btnBrowse;
    QLabel *lblComment;
    QLineEdit *txtComment;
    QLabel *lblArgs;
    QLineEdit *txtArgs;
    QPushButton *btnOK;
    QPushButton *btnCancel;
};

class NewFolderDialog : public QDialog
{
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

#endif // DIALOGS_H
