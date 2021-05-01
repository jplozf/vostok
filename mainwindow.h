#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "moviewidget.h"
#include "settings.h"
#include "settings.h"
#include "constants.h"
#include "dialogs.h"
#include "myfilesystemmodel.h"
#include "shortcuts.h"
#include "interpreter.h"

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
    void showMessage(QString);
    int currentTabIndex = 0;
    QString appDir;
    QString launcherDir;
    QString currentLauncherDir;
    Settings *appSettings;
    Constants *appConstants;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    void slotMinimized();
    void slotRestored();
    void slotMaximized();
    void slotClosed();
    void slotClock();
    void slotPin();
    void slotSettings();
    void slotOff();
    void slotLock();
    void slotEnter();
    void onTabChanged(int);
    void onContextMenuLauncher(QPoint);
    void onLaunchItemClicked(QModelIndex);
    void displayOSD(QString out);

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
};

#endif // MAINWINDOW_H
