#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moviewidget.h"
#include "settings.h"
#include "constants.h"
#include <QDebug>
#include <string>


//******************************************************************************
// LauncherWindow()
//******************************************************************************
LauncherWindow::LauncherWindow(QWidget *parent) : QMainWindow(parent, Qt::CustomizeWindowHint ) {
    appSettings = new Settings();
    appConstants = new Constants();

    //****************************************************************************
    // Create application folder if not exists
    //****************************************************************************
    appDir = QDir(QDir::homePath()).filePath(appConstants->getQString("APP_FOLDER"));
    if (!QDir(appDir).exists()) {
        QDir().mkdir(appDir);
    }

    //****************************************************************************
    // Create launcher subfolder if not exists
    //****************************************************************************
    launcherDir = QDir(appDir).filePath(appConstants->getQString("LAUNCHER_FOLDER"));
    if (!QDir(launcherDir).exists()) {
        QDir().mkdir(launcherDir);
    }
    currentLauncherDir = launcherDir;

    setObjectName("borderlessMainWindow");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::Tool);
    // to fix taskbar minimize feature
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_QuitOnClose, true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setStyleSheet("#borderlessMainWindow{border:1px solid palette(highlight);}");


    mMainWindow = new MainWindow(this);
    setWindowTitle(mMainWindow->windowTitle());

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(1);

    QHBoxLayout *windowBarLayout = new QHBoxLayout();
    windowBarLayout->setSpacing(0);
    windowBarLayout->setMargin(0);

    mTitlebarWidget = new QWidget(this);
    mTitlebarWidget->setObjectName("titlebarWidget");
    mTitlebarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mTitlebarWidget->setLayout(windowBarLayout);

    /*
    btnMinimize = new QPushButton(mTitlebarWidget);
    btnMinimize->setObjectName("minimizeButton");
    connect(btnMinimize, SIGNAL(clicked()), this, SLOT(slot_minimized()));

    btnRestore = new QPushButton(mTitlebarWidget);
    btnRestore->setObjectName("restoreButton");
    btnRestore->setVisible(false);
    connect(btnRestore, SIGNAL(clicked()), this, SLOT(slot_restored()));

    btnMaximize = new QPushButton(mTitlebarWidget);
    btnMaximize->setObjectName("maximizeButton");
    connect(btnMaximize, SIGNAL(clicked()), this, SLOT(slot_maximized()));

    btnClose = new QPushButton(mTitlebarWidget);
    btnClose->setObjectName("closeButton");
    btnClose->setIcon(QIcon(":/png/16x16/Close.png"));
    connect(btnClose, SIGNAL(clicked()), this, SLOT(slot_closed()));
    */

    btnClock = new QPushButton(mTitlebarWidget);
    btnClock->setObjectName("clockButton");
    btnClock->setFlat(true);
    btnClock->setIcon(QIcon(":/png/16x16/Clock.png"));
    connect(btnClock, SIGNAL(clicked()), this, SLOT(slotClock()));

    btnPin = new QPushButton(mTitlebarWidget);
    btnPin->setObjectName("pinButton");
    btnPin->setFlat(true);
    btnPin->setIcon(QIcon(":/png/16x16/Attach.png"));
    connect(btnPin, SIGNAL(clicked()), this, SLOT(slotPin()));

    btnSettings = new QPushButton(mTitlebarWidget);
    btnSettings->setObjectName("settingsButton");
    btnSettings->setFlat(true);
    btnSettings->setIcon(QIcon(":/png/16x16/Gear.png"));
    connect(btnSettings, SIGNAL(clicked()), this, SLOT(slotSettings()));

    btnLock = new QPushButton(mTitlebarWidget);
    btnLock->setObjectName("lockButton");
    btnLock->setFlat(true);
    btnLock->setIcon(QIcon(":/png/16x16/Lock.png"));
    connect(btnLock, SIGNAL(clicked()), this, SLOT(slotLock()));

    btnOff = new QPushButton(mTitlebarWidget);
    btnOff->setObjectName("offButton");
    btnOff->setFlat(true);
    btnOff->setIcon(QIcon(":/png/16x16/Standby.png"));
    connect(btnOff, SIGNAL(clicked()), this, SLOT(slotOff()));

    lblTitle = new QLabel(mTitlebarWidget);
    lblTitle->setObjectName("windowTitle");
    lblTitle->setText("Hello");

    txtCommand = new QLineEdit(mTitlebarWidget);
    txtCommand->setObjectName("commandLine");
    txtCommand->setText("");
    connect(txtCommand, SIGNAL(returnPressed()), this, SLOT(slotEnter()));

    windowBarLayout->addWidget(txtCommand);
    windowBarLayout->addWidget(lblTitle);

    // windowBarLayout->addStretch(1);
    /*
    windowBarLayout->addWidget(btnMinimize);
    windowBarLayout->addWidget(btnRestore);
    windowBarLayout->addWidget(btnMaximize);
    windowBarLayout->addWidget(btnClose);
    */
    windowBarLayout->addWidget(btnClock);
    windowBarLayout->addWidget(btnPin);
    windowBarLayout->addWidget(btnSettings);
    windowBarLayout->addWidget(btnLock);
    windowBarLayout->addWidget(btnOff);

    if (appSettings->getBool("TITLE_BAR_POSITION_TOP")) {
        verticalLayout->addWidget(mTitlebarWidget);
        verticalLayout->addWidget(mMainWindow);
    }
    else {
        verticalLayout->addWidget(mMainWindow);
        verticalLayout->addWidget(mTitlebarWidget);
    }

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    centralWidget->setLayout(verticalLayout);

    connect(mMainWindow->ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

    //****************************************************************************
    // Display clock timer
    //****************************************************************************
    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() {
        QString time1 = QDateTime::currentDateTime().toString(appSettings->getQString("TITLE_BAR_TIME_FORMAT"));
        lblTitle->setText(time1);
    } );
    t->start();

    setCentralWidget(centralWidget);
    readSettings();

    initLauncher();
    showMessage("Welcome");
}

//******************************************************************************
// tabChanged()
//******************************************************************************
void LauncherWindow::onTabChanged(int iTab) {
    if (currentTabIndex == 3 && mMainWindow->movieWidget->isVideoAvailable == true) {
        mMainWindow->movieWidget->pauseForce();
    }
    if (iTab == 3) {
        if (mMainWindow->movieWidget->isPlaying == true && mMainWindow->movieWidget->isVideoAvailable == true) {
            mMainWindow->movieWidget->playForce();
        }
    }
    currentTabIndex = iTab;
}

//******************************************************************************
// closeEvent()
//******************************************************************************
void LauncherWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    QMainWindow::closeEvent(event);
}

//******************************************************************************
// saveSettings()
//******************************************************************************
void LauncherWindow::saveSettings() {
    QSettings settings(appConstants->getQString("ORGANIZATION_NAME"), appConstants->getQString("APPLICATION_NAME"));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("splitFocus", mMainWindow->ui->tabVideoSplitter->saveState());
    settings.setValue("currentTab", mMainWindow->ui->tabWidget->currentIndex());
    settings.setValue("currentMedia", mMainWindow->movieWidget->currentMedia);
    settings.setValue("currentSession", mMainWindow->movieWidget->file_1_url_2);
    settings.setValue("currentPosition", mMainWindow->movieWidget->mediaPlayer->position());

    //**************************************************************************
    // Notepad saving
    //**************************************************************************
    QFile fNotepad(QDir(appDir).filePath(appConstants->getQString("NOTEPAD_FILE")));
    fNotepad.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    fNotepad.write(mMainWindow->ui->txtNotepad->toPlainText().toUtf8());
    fNotepad.close();

    //**************************************************************************
    // Focus saving
    //**************************************************************************
    QFile fFocus(QDir(appDir).filePath(appConstants->getQString("FOCUS_FILE")));
    fFocus.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    fFocus.write(mMainWindow->ui->txtFocus->toPlainText().toUtf8());
    fFocus.close();
}

//******************************************************************************
// readSettings()
//******************************************************************************
void LauncherWindow::readSettings() {
    QSettings settings(appConstants->getQString("ORGANIZATION_NAME"), appConstants->getQString("APPLICATION_NAME"));

    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    const QByteArray windowState = settings.value("windowState", QByteArray()).toByteArray();
    if (!windowState.isEmpty()) {
        restoreState(settings.value("windowState").toByteArray());
    }

    const QByteArray splitFocus = settings.value("splitFocus", QByteArray()).toByteArray();
    if (!splitFocus.isEmpty()) {
        mMainWindow->ui->tabVideoSplitter->restoreState(settings.value("splitFocus").toByteArray());
    }

    const int currentTab = settings.value("currentTab").toInt();
    if (currentTab >= 0) {
        mMainWindow->ui->tabWidget->setCurrentIndex(currentTab);
        currentTabIndex = currentTab;
    }

    const int currentSession = settings.value("currentSession").toInt();
    const QString currentMedia = settings.value("currentMedia").toString();
    const qint64 currentPosition = settings.value("currentPosition").toUInt();
    bool currentRun = false;
    if (currentMedia != "" && currentSession != 0) {
        if (currentTab == 3) {
            currentRun = true;
        }
        mMainWindow->movieWidget->restoreSession(currentMedia, currentSession, currentPosition, currentRun);
    }

    //**************************************************************************
    // Notepad restoring
    //**************************************************************************
    QFile fNotepad(QDir(appDir).filePath(appConstants->getQString("NOTEPAD_FILE")));
    fNotepad.open(QFile::ReadOnly | QFile::Text);
    mMainWindow->ui->txtNotepad->setText(fNotepad.readAll());
    fNotepad.close();

    //**************************************************************************
    // Focus restoring
    //**************************************************************************
    QFile fFocus(QDir(appDir).filePath(appConstants->getQString("FOCUS_FILE")));
    fFocus.open(QFile::ReadOnly | QFile::Text);
    mMainWindow->ui->txtFocus->setText(fFocus.readAll());
    fFocus.close();
}

//******************************************************************************
// mousePressEvent()
//******************************************************************************
void LauncherWindow::mousePressEvent(QMouseEvent* event) {
    if (!mTitlebarWidget->underMouse() && !lblTitle->underMouse())
        return;

    if(event->button() == Qt::LeftButton) {
        mMoving = true;
        mLastMousePosition = event->pos();
    }
}

//******************************************************************************
// mouseMoveEvent()
//******************************************************************************
void LauncherWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!mTitlebarWidget->underMouse() && !lblTitle->underMouse())
        return;

    if( event->buttons().testFlag(Qt::LeftButton) && mMoving) {
        this->move(this->pos() + (event->pos() - mLastMousePosition));
    }
}

//******************************************************************************
// mouseReleaseEvent()
//******************************************************************************
void LauncherWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (!mTitlebarWidget->underMouse() && !lblTitle->underMouse())
        return;

    if(event->button() == Qt::LeftButton) {
        mMoving = false;
    }
}

//******************************************************************************
// mouseDoubleClickEvent()
//******************************************************************************
void LauncherWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (!mTitlebarWidget->underMouse() && !lblTitle->underMouse())
        return;
    if (mMainWindow->isVisible()==true) {
        this->layout()->setSizeConstraint(QLayout::SetFixedSize);
        mMainWindow->setVisible(false);
    } else {
        this->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);
        mMainWindow->setVisible(true);
    }
/*
    mMaximized = !mMaximized;
    if (mMaximized) {
        slotMaximized();
    } else {
        slotRestored();
    }
*/
}

//******************************************************************************
// slotMinimized()
//******************************************************************************
void LauncherWindow::slotMinimized() {
    setWindowState(Qt::WindowMinimized);
}

//******************************************************************************
// slotRestored()
//******************************************************************************
void LauncherWindow::slotRestored() {
    btnRestore->setVisible(false);
    btnMaximize->setVisible(true);
    setWindowState(Qt::WindowNoState);
    setStyleSheet("#borderlessMainWindow{border:1px solid palette(highlight);}");
}

//******************************************************************************
// slotMaximized()
//******************************************************************************
void LauncherWindow::slotMaximized() {
    btnRestore->setVisible(true);
    btnMaximize->setVisible(false);
    setWindowState(Qt::WindowMaximized);
    setStyleSheet("#borderlessMainWindow{border:1px solid palette(base);}");
}

//******************************************************************************
// slotEnter()
//******************************************************************************
void LauncherWindow::slotEnter() {
    QString cmd = txtCommand->text();
    lblTitle->setText(QString("Yo %1").arg(cmd));
}

//******************************************************************************
// slotClosed()
//******************************************************************************
void LauncherWindow::slotClosed() {
    close();
}

//******************************************************************************
// slotClock()
//******************************************************************************
void LauncherWindow::slotClock() {
    // TODO : Clock
}

//******************************************************************************
// slotPin()
//******************************************************************************
void LauncherWindow::slotPin() {
    if (mMainWindow->isVisible()==true) {
        saveSettings();
        this->layout()->setSizeConstraint(QLayout::SetFixedSize);
        if (currentTabIndex == 3 && mMainWindow->movieWidget->isVideoAvailable == true) {
            mMainWindow->movieWidget->pauseForce();
        }
        mMainWindow->setVisible(false);
    } else {
        this->layout()->setSizeConstraint(QLayout::SetMinAndMaxSize);        
        mMainWindow->setVisible(true);
        readSettings();
        if (currentTabIndex == 3 && mMainWindow->movieWidget->isVideoAvailable == true) {
            if (mMainWindow->movieWidget->isPlaying == true) {
                mMainWindow->movieWidget->playForce();
            }
        }
    }
}

//******************************************************************************
// slotLock()
//******************************************************************************
void LauncherWindow::slotLock() {
    // rundll32.exe user32.dll,LockWorkStation
    // dbus-send --type=method_call --dest=org.mate.ScreenSaver /org/mate/ScreenSaver org.mate.ScreenSaver.Lock
    QProcess *process = new QProcess(this);
#ifdef Q_OS_LINUX
    QString program = "dbus-send --type=method_call --dest=org.mate.ScreenSaver /org/mate/ScreenSaver org.mate.ScreenSaver.Lock";
    process->start(program);
#endif
#ifdef Q_OS_WIN32
    QString program = "rundll32.exe user32.dll,LockWorkStation";
    process->start(program);
#endif
}

//******************************************************************************
// slotOff()
//******************************************************************************
void LauncherWindow::slotOff() {
    // TODO : Off
    close();
}

//******************************************************************************
// slotSettings()
//******************************************************************************
void LauncherWindow::slotSettings() {
    // TODO : Settings
}

//******************************************************************************
// displaySettings()
//******************************************************************************
void LauncherWindow::displaySettings() {
    // TODO : Display Settings
}

//******************************************************************************
// showMessage()
//******************************************************************************
void LauncherWindow::showMessage(QString msg) {
    mMainWindow->ui->statusbar->showMessage(msg, appSettings->getInt("MESSAGE_TIMEOUT"));
}

//******************************************************************************
// initLauncher()
//******************************************************************************
void LauncherWindow::initLauncher() {
    //**************************************************************************
    // Set FileSystem Model for Launcher
    //**************************************************************************
    listModel = new QFileSystemModel(this);
    listModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    mMainWindow->ui->lstLauncher->setModel(listModel);
    mMainWindow->ui->lstLauncher->setRootIndex(listModel->setRootPath(launcherDir));

    //**************************************************************************
    // Bind Context Menus for Launcher
    //**************************************************************************
    mMainWindow->ui->lstLauncher->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mMainWindow->ui->lstLauncher, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onContextMenuLauncher(QPoint)));
    connect(mMainWindow->ui->lstLauncher, SIGNAL(clicked(QModelIndex)), SLOT(onLaunchItemClicked(QModelIndex)));

    //**************************************************************************
    // Set Context Menu for Launcher
    //**************************************************************************
    mnuLauncher = new QMenu(mMainWindow->ui->lstLauncher);

    mnuNew = new QMenu(tr("New..."), mMainWindow->ui->lstLauncher);
    // mnuNew->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)));
    mnaNewFolder = new QAction(tr("Folder"), mMainWindow->ui->lstLauncher);
    mnaNewShortcut = new QAction(tr("Shortcut"), mMainWindow->ui->lstLauncher);
    mnuNew->addAction(mnaNewFolder);
    mnuNew->addAction(mnaNewShortcut);

    mnuDisplay = new QMenu(tr("Display..."), mMainWindow->ui->lstLauncher);
    // mnuDisplay->setIcon(QIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)));
    mnaDisplayIcon = new QAction(tr("Icon"), mMainWindow->ui->lstLauncher);
    mnaDisplayList = new QAction(tr("List"), mMainWindow->ui->lstLauncher);
    mnuDisplay->addAction(mnaDisplayIcon);
    mnuDisplay->addAction(mnaDisplayList);

    mnuLauncher->addMenu(mnuDisplay);
    mnuLauncher->addMenu(mnuNew);

    //**************************************************************************
    // Set Context Menu for List Items
    //**************************************************************************
    mnuList = new QMenu(mMainWindow->ui->lstLauncher);
    mnaListRename = new QAction(tr("Rename..."), mMainWindow->ui->lstLauncher);
    mnaListDelete = new QAction(tr("Delete..."), mMainWindow->ui->lstLauncher);
    mnaListEdit = new QAction(tr("Edit..."), mMainWindow->ui->lstLauncher);

    mnuList->addAction(mnaListRename);
    mnuList->addAction(mnaListDelete);
    mnuList->addAction(mnaListEdit);
}

//******************************************************************************
// openContextMenuLauncher()
//******************************************************************************
void LauncherWindow::onContextMenuLauncher(QPoint position) {
    if(!((mMainWindow->ui->lstLauncher->selectionModel()->selectedIndexes()).empty()))
    {
        //**********************************************************************
        // Context Menu for Items
        //**********************************************************************
        QAction *action = mnuList->exec(mMainWindow->ui->lstLauncher->viewport()->mapToGlobal(position));
        if (action == mnaListRename) {
            // TODO : Rename
        } else {
            if (action == mnaListDelete) {
                // TODO : Delete
            } else {
                if (action == mnaListEdit) {
                    // TODO : Edit
                }
            }
        }
        //Select clearly
        mMainWindow->ui->lstLauncher->selectionModel()->clear();
    } else {
        //**********************************************************************
        // Context Menu for List View
        //**********************************************************************
        QAction *action = mnuLauncher->exec(mMainWindow->ui->lstLauncher->viewport()->mapToGlobal(position));
        if (action == mnaNewShortcut) {
            newShortcut();
        } else {
            if (action == mnaNewFolder) {
                newFolder();
            } else {
                if (action == mnaDisplayIcon) {
                    mMainWindow->ui->lstLauncher->setViewMode(QListView::IconMode);
                    // displayLauncher();
                } else {
                    if (action == mnaDisplayList) {
                        mMainWindow->ui->lstLauncher->setViewMode(QListView::ListMode);
                        // displayLauncher();
                    }
                }
            }
        }
    }
}

//******************************************************************************
// newShortcut()
//******************************************************************************
void LauncherWindow::newShortcut() {
    NewShortcutDialog dlg = new NewShortcutDialog(this);
    int rc = dlg.exec();
    if (rc == QDialog::Accepted) {
        createShortcut(&dlg);
    }
}

//******************************************************************************
// createShortcut()
//******************************************************************************
void LauncherWindow::createShortcut(NewShortcutDialog *dlg) {
    QString shortcutName = dlg->getShortcutName();
    QString shortcutCommand = dlg->getShortcutCommand();
    QString shortcutArgs = dlg->getShortcutArgs();
#ifdef Q_OS_LINUX
    // TODO : Create Linux Shortcut
    QString shortcutFile =  QDir(currentLauncherDir).filePath(shortcutName);

#endif
#ifdef Q_OS_WIN32
    // TODO : Create Windows Shortcut
#endif
}

//******************************************************************************
// newFolder()
//******************************************************************************
void LauncherWindow::newFolder() {
    NewFolderDialog dlg = new NewFolderDialog(this);
    int rc = dlg.exec();
    if (rc == QDialog::Accepted) {
        QString folderName = dlg.getFolderName();
        QString folderDir = QDir(currentLauncherDir).filePath(folderName);
        if (!QDir(folderDir).exists()) {
            QDir().mkdir(folderDir);
        }
    }
}

//******************************************************************************
// onLaunchItemClicked()
//******************************************************************************
void LauncherWindow::onLaunchItemClicked(QModelIndex index)
{
    currentLauncherDir = listModel->fileInfo(index).absoluteFilePath();
    if (currentLauncherDir == launcherDir) {
        listModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    } else {
        listModel->setFilter(QDir::AllEntries | QDir::NoDot);
    }
    mMainWindow->ui->lstLauncher->setRootIndex(listModel->setRootPath(currentLauncherDir));
    showMessage(currentLauncherDir);
}

//******************************************************************************
// MainWindow()
//******************************************************************************
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_QuitOnClose, true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);
    statusBar()->setSizeGripEnabled(true);
    ui->txtNotepad->setText("Hello");
    movieWidget = new MovieWidget(this);
    ui->tabVideoSplitter->addWidget(movieWidget);
}

//******************************************************************************
// ~MainWindow()
//******************************************************************************
MainWindow::~MainWindow()
{
    delete ui;
}
