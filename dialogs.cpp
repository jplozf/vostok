#include "dialogs.h"

const int PowerDialog::CANCEL = 1;
const int PowerDialog::LOCK = 2;
const int PowerDialog::SHUTDOWN = 3;
const int PowerDialog::REBOOT = 4;
const int PowerDialog::EXIT = 5;

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
    // QLabel *lblMessage = new QLabel(msg->toLatin1());
    OSDLabel *lblMessage = new OSDLabel(msg);

    /*
    QFont f( "Courier", mySettings->get("OSD_FONT_SIZE").toInt(), QFont::Bold);
    lblMessage->setFont( f);
    lblMessage->setStyleSheet("QLabel { color : " + mySettings->get("OSD_COLOR").toString() + "; }");

    QGraphicsDropShadowEffect* fx1 = new QGraphicsDropShadowEffect(this);
    fx1->setOffset(2, 2);
    fx1->setColor(QColor(mySettings->get("OSD_SHADOW_COLOR").toString()));
    lblMessage->setGraphicsEffect(fx1);
    */

    layout->addWidget(lblMessage);
    setLayout(layout);
    parent->setFocus();
}

OSDDialog::~OSDDialog()
{
    if(tmrDisplay != Q_NULLPTR)
        tmrDisplay->deleteLater();
}

//******************************************************************************
// OSDLabel()
//******************************************************************************
OSDLabel::OSDLabel(QWidget *parent) : QLabel(parent) {

}

//******************************************************************************
// OSDLabel()
//******************************************************************************
OSDLabel::OSDLabel(QString *text) : QLabel(*text) {

}

//******************************************************************************
// paintEvent()
//******************************************************************************
void OSDLabel::paintEvent(QPaintEvent *) {
    int off = 2;
    QPainter painter(this);
    QPainterPath path;
    QFont drawFont = QFont("Sans", 10);
    Settings *mySettings = new Settings();
    path.addText(off, drawFont.pointSize() + off, drawFont, text());
    painter.setRenderHints(QPainter::Antialiasing);
    painter.strokePath(path, QPen(QColor(mySettings->get("OSD_SHADOW_COLOR").toString()), 2));
    painter.fillPath(path, QBrush(QColor(mySettings->get("OSD_COLOR").toString())));
    resize(path.boundingRect().size().toSize().width() + off * 5, path.boundingRect().size().toSize().height() + off * 5);
}

//******************************************************************************
// PowerDialog()
//******************************************************************************
PowerDialog::PowerDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Power down the computer");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    QFormLayout *form = new QFormLayout(this);

    // Line #1 (Lock)
    btnLock = new QPushButton();
    btnLock->setIcon(QIcon(":/png/16x16/Lock.png"));
    form->addRow(tr("Lock this session"), btnLock);

    QFrame *qf1 = new QFrame(this);
    qf1->setFrameShape(QFrame::HLine);
    qf1->setFrameShadow(QFrame::Sunken);
    form->addRow(qf1);

    // Line #2 (Shutdown)
    btnShutdown = new QPushButton();
    btnShutdown->setIcon(QIcon(":/png/16x16/Shutdown.png"));
    form->addRow(tr("Shutdown the computer"), btnShutdown);

    // Line #3 (Reboot)
    btnReboot = new QPushButton();
    btnReboot->setIcon(QIcon(":/png/16x16/Rotate.png"));
    form->addRow(tr("Reboot the computer"), btnReboot);

    // Line #4 (Timeout)
    spnTimeout = new QSpinBox(this);
    spnTimeout->setRange(0, 1440);
    spnTimeout->setValue(1);
    form->addRow(tr("Timeout (in minutes)"), spnTimeout);

    QFrame *qf2 = new QFrame(this);
    qf2->setFrameShape(QFrame::HLine);
    qf2->setFrameShadow(QFrame::Sunken);
    form->addRow(qf2);

    // Line #5 (Exit)
    btnExit = new QPushButton();
    btnExit->setIcon(QIcon(":/png/16x16/Hand.png"));
    form->addRow(tr("Exit from Vostok launcher"), btnExit);

    QFrame *qf3 = new QFrame(this);
    qf3->setFrameShape(QFrame::HLine);
    qf3->setFrameShadow(QFrame::Sunken);
    form->addRow(qf3);

    // Line #6 (Cancel)
    btnCancel = new QPushButton();
    btnCancel->setIcon(QIcon(":/png/16x16/Close.png"));
    form->addRow(tr("Cancel"), btnCancel);


    this->setLayout(form);

    // set signal and slot for "Buttons"
    connect(btnLock, SIGNAL(clicked()), this, SLOT(returnLock()));
    connect(btnShutdown, SIGNAL(clicked()), this, SLOT(returnShutdown()));
    connect(btnReboot, SIGNAL(clicked()), this, SLOT(returnReboot()));
    connect(btnExit, SIGNAL(clicked()), this, SLOT(returnExit()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(returnCancel()));
}

//******************************************************************************
// getTimeout()
//******************************************************************************
int PowerDialog::getTimeout() {
    return(spnTimeout->value());
}

//******************************************************************************
// returnLock()
//******************************************************************************
void PowerDialog::returnLock() {
    this->done(PowerDialog::LOCK);
}

//******************************************************************************
// returnShutdown()
//******************************************************************************
void PowerDialog::returnShutdown() {
    this->done(PowerDialog::SHUTDOWN);
}

//******************************************************************************
// returnReboot()
//******************************************************************************
void PowerDialog::returnReboot() {
    this->done(PowerDialog::REBOOT);
}

//******************************************************************************
// returnExit()
//******************************************************************************
void PowerDialog::returnExit() {
    this->done(PowerDialog::EXIT);
}

//******************************************************************************
// returnCancel()
//******************************************************************************
void PowerDialog::returnCancel() {
    this->done(PowerDialog::CANCEL);
}

//******************************************************************************
// done()
// override the QDialog::done() and return RC
//******************************************************************************
void PowerDialog::done(int rc) {
    QDialog::done(rc);
}


//******************************************************************************
// AlarmsDialog()
//******************************************************************************
AlarmsDialog::AlarmsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Alarms management");
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    setWindowModality(Qt::WindowModal);

    // Global dialog layout
    QVBoxLayout *vLayout = new QVBoxLayout(this);

    // Alarms list
    tblAlarms = new QTableWidget();
    tblAlarms->setColumnCount(13);
    tblAlarms->setHorizontalHeaderLabels(QStringList()
                                            << "Enabled"
                                            << "Time"
                                            << "Message"
                                            << "Monday"
                                            << "Tuesday"
                                            << "Wednesday"
                                            << "Thursday"
                                            << "Friday"
                                            << "Saturday"
                                            << "Sunday"
                                            << "Sound"
                                            << "Edit"
                                            << "Delete"
                                            );

    QList<Alarms::alarm> *alarms;
    Alarms ac;
    alarms = ac.getAlarms();
    for(int i = 0; i < alarms->count(); ++i )
    {
        // qDebug() << alarms->at(i).message;
        tblAlarms->insertRow(tblAlarms->rowCount());

        tblAlarms->setCellWidget ( i, 0, new QCheckBox() );
        // tblAlarms->setItem(i, 0, new QTableWidgetItem(new QCheckBox()));
        tblAlarms->setItem(i, 1, new QTableWidgetItem(alarms->at(i).time));
        tblAlarms->setItem(i, 2, new QTableWidgetItem(alarms->at(i).message));
        tblAlarms->setItem(i, 3, new QTableWidgetItem(alarms->at(i).monday));
        tblAlarms->setItem(i, 4, new QTableWidgetItem(alarms->at(i).tuesday));
        tblAlarms->setItem(i, 5, new QTableWidgetItem(alarms->at(i).wednesday));
        tblAlarms->setItem(i, 6, new QTableWidgetItem(alarms->at(i).thursday));
        tblAlarms->setItem(i, 7, new QTableWidgetItem(alarms->at(i).friday));
        tblAlarms->setItem(i, 8, new QTableWidgetItem(alarms->at(i).saturday));
        tblAlarms->setItem(i, 9, new QTableWidgetItem(alarms->at(i).sunday));
        tblAlarms->setItem(i, 10, new QTableWidgetItem(alarms->at(i).sound));


    }
    tblAlarms->resizeColumnsToContents();
    vLayout->addWidget(tblAlarms);

    // New alarm layout
    QFormLayout *form = new QFormLayout();

    // Line #1 (Enabled)
    chkEnabled = new QCheckBox("Enabled");
    form->addRow(nullptr , chkEnabled);

    // Line #2 (Time)
    tmeAlarm = new QTimeEdit(QTime::currentTime());
    tmeAlarm->setTimeRange(QTime(0, 0, 0, 0), QTime(23, 59, 0, 0));
    form->addRow(tr("Alarm time"), tmeAlarm);

    // Line #3 (Days)
    QVBoxLayout *vlDays = new QVBoxLayout(this);
    chkMonday = new QCheckBox("Monday");
    vlDays->addWidget(chkMonday);
    chkTuesday = new QCheckBox("Tuesday");
    vlDays->addWidget(chkTuesday);
    chkWednesday = new QCheckBox("Wednesday");
    vlDays->addWidget(chkWednesday);
    chkThursday = new QCheckBox("Thursday");
    vlDays->addWidget(chkThursday);
    chkFriday = new QCheckBox("Friday");
    vlDays->addWidget(chkFriday);
    chkSaturday = new QCheckBox("Saturday");
    vlDays->addWidget(chkSaturday);
    chkSunday = new QCheckBox("Sunday");
    vlDays->addWidget(chkSunday);
    form->addRow(tr("Alarm days"), vlDays);

    // Line #4 (Message)
    txtMessage = new QLineEdit(this);
    form->addRow(tr("Message"), txtMessage);

    // Line #5 (Sound)
    btnPlay = new QPushButton("Sound");
    lstSounds = new QComboBox(this);
    QDirIterator it(":/sounds/alarms/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString item = it.next();
        qDebug() << item;
        QString f = QFileInfo(item).baseName();
        lstSounds->addItem(f, item);
    }
    form->addRow(btnPlay, lstSounds);

    // Last line (OK & Cancel Buttons)
    QHBoxLayout *h1 = new QHBoxLayout(this);
    QSpacerItem *s1 = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnCancel = new QPushButton("Cancel");
    btnOK = new QPushButton("OK");
    btnOK->setDefault(true);
    h1->addSpacerItem(s1);
    h1->addWidget(btnCancel);
    h1->addWidget(btnOK);
    form->addRow(nullptr, h1);

    vLayout->addLayout(form);

    this->setLayout(vLayout);

    // set signal and slot for "Buttons"
    connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnPlay, SIGNAL(clicked()), this, SLOT(playSound()));
}

//******************************************************************************
// playSound()
//******************************************************************************
void AlarmsDialog::playSound() {
    QString fSound = lstSounds->currentData().toString();
    QSound::play(fSound);
}

//******************************************************************************
// done()
//******************************************************************************
void AlarmsDialog::done(int rc) {
    if(rc == QDialog::Accepted)
    {
        a.enabled = chkEnabled->isChecked();
        a.time = tmeAlarm->text();
        a.monday = chkMonday->isChecked();
        a.tuesday = chkTuesday->isChecked();
        a.wednesday = chkWednesday->isChecked();
        a.thursday = chkThursday->isChecked();
        a.friday = chkFriday->isChecked();
        a.saturday = chkSaturday->isChecked();
        a.sunday = chkSunday->isChecked();
        a.message = txtMessage->text();
        a.sound = lstSounds->currentData().toString();
    }
    QDialog::done(rc);
}
