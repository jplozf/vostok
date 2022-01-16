#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QMainWindow>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QSize>
#include "moviewidget.h"
#include "settings.h"
#include "constants.h"
#include "dialogs.h"
#include "myfilesystemmodel.h"
#include "shortcuts.h"
#include "rpn.h"
#include "ui_mainwindow.h"
#include "moviewidget.h"
#include "alarms.h"
#include <QDebug>
#include <string>

#ifdef Q_OS_LINUX
#include <QtX11Extras/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//******************************************************************************
// Class MainWindow
//******************************************************************************
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    MovieWidget *movieWidget;
};

//******************************************************************************
// Class LauncherWindow
//******************************************************************************
class LauncherWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit LauncherWindow(QWidget *parent = 0);
    ~LauncherWindow() {}
    void closeEvent(QCloseEvent *);
    void saveSettings();
    void readSettings();
    void showMessage(QString, bool timed = true);
    int currentTabIndex = 0;
    QString appDir;
    QString launcherDir;
    QString currentLauncherDir;
    Settings *appSettings;
    Constants *appConstants;
    RPN *rpn;
    Alarms *alarms;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    bool eventFilter(QObject* obj, QEvent *event);

private slots:
    void slotMinimized();
    void slotRestored();
    void slotMaximized();
    void slotClosed();
    void slotClock();
    void slotPin();
    void slotPack();
    void slotSettings();
    void slotOff();
    void slotLock();
    void slotEnter();
    void slotClearConsole();
    void slotOpenFile();
    void slotCloseFile();
    void onTabChanged(int);
    void onContextMenuLauncher(QPoint);
    void onLaunchItemClicked(QModelIndex);
    void alarmCheck();
    QScreen *getActiveScreen(QWidget* pWidget) const;

protected slots:
    void readLog();

private:
    MainWindow *mMainWindow;
    QHBoxLayout *windowBarLayout;
    QWidget *mTitlebarWidget;
    QLabel *lblTitle;
    QLineEdit *txtCommand;
    QPushButton *btnMinimize;
    QPushButton *btnRestore;
    QPushButton *btnMaximize;
    QPushButton *btnClose;
    QPushButton *btnClock;
    QPushButton *btnPin;
    QPushButton *btnPack;
    QPushButton *btnSettings;
    QPushButton *btnLock;
    QPushButton *btnOff;
    QPoint mLastMousePosition;
    bool mMoving;
    bool mMaximized;
    void displaySettings();
    void initLauncher();
    QMenu *mnuLauncher;
    QMenu *mnuNew;
    QAction *mnaNewFolder;
    QAction *mnaNewShortcut;
    QMenu *mnuDisplay;
    QAction *mnaDisplayIcon;
    QAction *mnaDisplayList;
    QMenu *mnuList;
    QAction *mnaListRename;
    QAction *mnaListDelete;
    QAction *mnaListEdit;
    void newShortcut();
    void newFolder();
    MyFileSystemModel *listModel;
    void createShortcut(NewShortcutDialog *);
    void editShortcut(Shortcut *);
    void updateShortcut(EditShortcutDialog *);
    void displayOSD(QString out);
    void displayOutput(QString cmd, QString out);
    void shutdownComputer(int timeout, bool reboot);
    void runCommand(QString dir, QString cmd, QTextEdit *view);
    QTimer timerLog;
    QFile logFile;
    QTextStream logStream;
    bool logMe;
    QProcess *pCmd;


};

#endif // MAINWINDOW_H
