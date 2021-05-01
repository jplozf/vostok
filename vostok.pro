QT       += core gui multimediawidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    constants.cpp \
    dialogs.cpp \
    interpreter.cpp \
    main.cpp \
    mainwindow.cpp \
    moviewidget.cpp \
    myfilesystemmodel.cpp \
    runguard.cpp \
    settings.cpp \
    shortcuts.cpp

HEADERS += \
    constants.h \
    dialogs.h \
    interpreter.h \
    mainwindow.h \
    moviewidget.h \
    myfilesystemmodel.h \
    runguard.h \
    settings.h \
    shortcuts.h

FORMS += \
    mainwindow.ui \
    newshortcut.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    vostok.qrc

unix: CONFIG += link_pkgconfig

DISTFILES += \
    dox
