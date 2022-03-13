#include "mainwindow.h"
#include "runguard.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    RunGuard guard("VOSTOK_BY_JPL");
    if (!guard.tryToRun()) {
        std::cout << "Program is already running." << std::endl;
        return 0;
    }

    QApplication a(argc, argv);

    LauncherWindow bw;
    bw.setWindowTitle("Vostok");
    bw.show();

    // TODO : Manage Dark Mode
    // a.setStyle("Fusion");
    // a.setPalette(QPalette(QPalette::Dark));

    return a.exec();
}

/*
 * https://stackoverflow.com/questions/2235360/making-a-borderless-window-with-for-qt
 * https://stackoverflow.com/questions/18765918/how-to-create-a-draggable-borderless-and-titleless-top-level-window-in-qt
 * https://stackoverflow.com/questions/11314429/select-moving-qwidget-in-the-screen
 * https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle
 */
