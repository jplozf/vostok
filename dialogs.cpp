#include "dialogs.h"

//******************************************************************************
// NewShortcutDialog()
//******************************************************************************
NewShortcutDialog::NewShortcutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("New Shortcut");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    QFormLayout *form = new QFormLayout(this);

    // Line #1 (Name)
    lblName = new QLabel("Name");
    txtName = new QLineEdit("");
    form->addRow(lblName, txtName);

    // Line #2 (Command)
    QHBoxLayout *h1 = new QHBoxLayout(this);
    lblCommand = new QLabel("Command");
    txtCommand = new QLineEdit("");
    btnBrowse = new QPushButton("Browse...");
    h1->addWidget(txtCommand);
    h1->addWidget(btnBrowse);
    form->addRow(lblCommand, h1);

    // Line #3 (Arguments)
    lblArgs = new QLabel("Arguments");
    txtArgs = new QLineEdit("");
    form->addRow(lblArgs, txtArgs);

    /*
    // Line #3 (Informations)
    lblComment = new QLabel("Informations");
    txtComment = new QLineEdit("");
    form->addRow(lblComment, txtComment);
    */

    // Line #4 (Vertical spacer)
    QVBoxLayout *v1 = new QVBoxLayout(this);
    v1->addStretch();
    form->addRow(nullptr, v1);

    // Line #5 (OK & Cancel Buttons)
    QHBoxLayout *h2 = new QHBoxLayout(this);
    QSpacerItem *s2 = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnCancel = new QPushButton("Cancel");
    btnOK = new QPushButton("OK");
    btnOK->setDefault(true);
    h2->addSpacerItem(s2);
    h2->addWidget(btnCancel);
    h2->addWidget(btnOK);
    form->addRow(nullptr, h2);

    this->setLayout(form);

    // set signal and slot for "Buttons"
    connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

//******************************************************************************
// getShortcutName()
//******************************************************************************
QString NewShortcutDialog::getShortcutName() {
    return (this->txtName->text());
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString NewShortcutDialog::getShortcutCommand() {
    return (this->txtCommand->text());
}

//******************************************************************************
// getShortcutArgs()
//******************************************************************************
QString NewShortcutDialog::getShortcutArgs() {
    return (this->txtArgs->text());
}

//******************************************************************************
// NewFolderDialog()
//******************************************************************************
NewFolderDialog::NewFolderDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("New Folder");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    QFormLayout *form = new QFormLayout(this);

    // Line #1 (Name)
    lblName = new QLabel("Name");
    txtName = new QLineEdit("");
    form->addRow(lblName, txtName);

    /*
    // Line #2 (Informations)
    lblComment = new QLabel("Informations");
    txtComment = new QLineEdit("");
    form->addRow(lblComment, txtComment);
    */

    // Line #3 (Vertical spacer)
    QVBoxLayout *v1 = new QVBoxLayout(this);
    v1->addStretch();
    form->addRow(nullptr, v1);

    // Line #4 (OK & Cancel Buttons)
    QHBoxLayout *h2 = new QHBoxLayout(this);
    QSpacerItem *s2 = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnCancel = new QPushButton("Cancel");
    btnOK = new QPushButton("OK");
    btnOK->setDefault(true);
    h2->addSpacerItem(s2);
    h2->addWidget(btnCancel);
    h2->addWidget(btnOK);
    form->addRow(nullptr, h2);

    this->setLayout(form);

    // set signal and slot for "Buttons"
    connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

//******************************************************************************
// getFolderName()
//******************************************************************************
QString NewFolderDialog::getFolderName() {
    return (this->txtName->text());
}
