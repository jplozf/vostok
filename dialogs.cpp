#include "dialogs.h"

//******************************************************************************
// NewShortcutDialog()
//******************************************************************************
NewShortcutDialog::NewShortcutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("New Shortcut");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    QFormLayout *form = new QFormLayout(this);

    // Line #1 (Type)
    lblType = new QLabel("Type");
    cbxType = new QComboBox();
    QStringList ls;
    ls<<"Application"<<"Application in terminal"<<"File or URL";
    cbxType->addItems(ls);
    form->addRow(lblType, cbxType);

    // Line #2 (Name)
    lblName = new QLabel("Name");
    txtName = new QLineEdit("");
    form->addRow(lblName, txtName);

    // Line #3 (Command)
    QHBoxLayout *h1 = new QHBoxLayout(this);
    lblCommand = new QLabel("Command");
    txtCommand = new QLineEdit("");
    btnBrowse = new QPushButton("Browse...");
    connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browseCommand()));
    h1->addWidget(txtCommand);
    h1->addWidget(btnBrowse);
    form->addRow(lblCommand, h1);

    // Line #4 (Informations)
    lblComment = new QLabel("Informations");
    txtComment = new QLineEdit("");
    form->addRow(lblComment, txtComment);

    // Line #5 (Icon)
    strIcon = ":/png/png/vostok_48.png";
    btnIcon = new QPushButton();
    btnIcon->setIcon(QIcon(strIcon));
    btnIcon->setIconSize(QSize(48, 48));
    connect(btnIcon, SIGNAL(clicked()), this, SLOT(browseIcon()));
    form->addRow(new QLabel("Icon"), btnIcon);

    // Line #5 (Vertical spacer)
    QVBoxLayout *v1 = new QVBoxLayout(this);
    v1->addStretch();
    form->addRow(nullptr, v1);

    // Line #7 (OK & Cancel Buttons)
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
// done()
// QDialog overridden done() method to validate input fields
//******************************************************************************
void NewShortcutDialog::done(int rc)
{
    if(rc == QDialog::Accepted)
    {
        // OK was pressed
        if(txtCommand->text() != "" && txtName->text() != "")
        {
            QDialog::done(rc);
            return;
        } else {
            return;
        }
    } else {
        // Cancel, Close or Esc was pressed
        QDialog::done(rc);
        return;
    }
}

//******************************************************************************
// browseCommand()
//******************************************************************************
void NewShortcutDialog::browseCommand() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open target"), QDir::homePath(), tr("All Files (*);;Movies (*.avi *.mp4);;Music (*.mp3 *.ogg)"), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename != "") {
        txtCommand->setText(filename);
    }
}

//******************************************************************************
// browseIcon()
//******************************************************************************
void NewShortcutDialog::browseIcon() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open target"), QDir::homePath(), tr("Icon (*.ico *.png *.jpeg *.jpg);;All Files (*)"), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename != "") {
        strIcon = filename;
        btnIcon->setIcon(QIcon(strIcon));
        btnIcon->setIconSize(QSize(48, 48));
    }
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
// getShortcutCommand()
//******************************************************************************
QString NewShortcutDialog::getShortcutIcon() {
    return (this->strIcon);
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString NewShortcutDialog::getShortcutComment() {
    return (this->txtComment->text());
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString NewShortcutDialog::getShortcutType() {
    return (this->cbxType->currentText());
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


//******************************************************************************
// NewShortcutDialog()
//******************************************************************************
EditShortcutDialog::EditShortcutDialog(QWidget *parent, Shortcut *sh) : QDialog(parent) {
    setWindowTitle("Edit Shortcut");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    QFormLayout *form = new QFormLayout(this);

    // Line #1 (Type)
    lblType = new QLabel("Type");
    cbxType = new QComboBox();
    QStringList ls;
    ls<<"Application"<<"Application in terminal"<<"File or URL";
    cbxType->addItems(ls);
    form->addRow(lblType, cbxType);

    // Line #2 (Name)
    lblName = new QLabel("Name");
    txtName = new QLineEdit(sh->name);
    form->addRow(lblName, txtName);

    // Line #3 (Command)
    QHBoxLayout *h1 = new QHBoxLayout(this);
    lblCommand = new QLabel("Command");
    txtCommand = new QLineEdit(sh->exec);
    btnBrowse = new QPushButton("Browse...");
    connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browseCommand()));
    h1->addWidget(txtCommand);
    h1->addWidget(btnBrowse);
    form->addRow(lblCommand, h1);

    // Line #4 (Informations)
    lblComment = new QLabel("Informations");
    txtComment = new QLineEdit(sh->comment);
    form->addRow(lblComment, txtComment);

    // Line #5 (Icon)
    strIcon = ":/png/png/vostok_48.png";
    btnIcon = new QPushButton();
    btnIcon->setIcon(QIcon(strIcon));
    btnIcon->setIconSize(QSize(48, 48));
    connect(btnIcon, SIGNAL(clicked()), this, SLOT(browseIcon()));
    form->addRow(new QLabel("Icon"), btnIcon);

    // Line #5 (Vertical spacer)
    QVBoxLayout *v1 = new QVBoxLayout(this);
    v1->addStretch();
    form->addRow(nullptr, v1);

    // Line #7 (OK & Cancel Buttons)
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
// done()
// QDialog overridden done() method to validate input fields
//******************************************************************************
void EditShortcutDialog::done(int rc)
{
    if(rc == QDialog::Accepted)
    {
        // OK was pressed
        if(txtCommand->text() != "" && txtName->text() != "")
        {
            QDialog::done(rc);
            return;
        } else {
            return;
        }
    } else {
        // Cancel, Close or Esc was pressed
        QDialog::done(rc);
        return;
    }
}

//******************************************************************************
// browseCommand()
//******************************************************************************
void EditShortcutDialog::browseCommand() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open target"), QDir::homePath(), tr("All Files (*);;Movies (*.avi *.mp4);;Music (*.mp3 *.ogg)"), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename != "") {
        txtCommand->setText(filename);
    }
}

//******************************************************************************
// browseIcon()
//******************************************************************************
void EditShortcutDialog::browseIcon() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open target"), QDir::homePath(), tr("Icon (*.ico *.png *.jpeg *.jpg);;All Files (*)"), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename != "") {
        strIcon = filename;
        btnIcon->setIcon(QIcon(strIcon));
        btnIcon->setIconSize(QSize(48, 48));
    }
}

//******************************************************************************
// getShortcutName()
//******************************************************************************
QString EditShortcutDialog::getShortcutName() {
    return (this->txtName->text());
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString EditShortcutDialog::getShortcutCommand() {
    return (this->txtCommand->text());
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString EditShortcutDialog::getShortcutIcon() {
    return (this->strIcon);
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString EditShortcutDialog::getShortcutComment() {
    return (this->txtComment->text());
}

//******************************************************************************
// getShortcutCommand()
//******************************************************************************
QString EditShortcutDialog::getShortcutType() {
    return (this->cbxType->currentText());
}

//******************************************************************************
// OSDDialog()
//******************************************************************************
OSDDialog::OSDDialog(QWidget *parent, QString *msg) : QDialog(parent) {
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_TranslucentBackground, true);

    tmrDisplay = new QTimer(this);
    tmrDisplay->setSingleShot(true);
    connect(tmrDisplay, &QTimer::timeout, this, [=](){this->close();});
    Settings *mySettings = new Settings();
    tmrDisplay->start(mySettings->get("OSD_TIMEOUT").toInt());

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *lblMessage = new QLabel(msg->toLatin1());
    QFont f( "Courier", mySettings->get("OSD_FONT_SIZE").toInt(), QFont::Bold);
    lblMessage->setFont( f);
    lblMessage->setStyleSheet("QLabel { color : " + mySettings->get("OSD_COLOR").toString() + "; }");

    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(2, 2);
    effect->setColor(QColor(mySettings->get("OSD_SHADOW_COLOR").toString()));
    lblMessage->setGraphicsEffect(effect);

    layout->addWidget(lblMessage);
    setLayout(layout);
    parent->setFocus();
}

OSDDialog::~OSDDialog()
{
    if(tmrDisplay != Q_NULLPTR)
        tmrDisplay->deleteLater();
}
