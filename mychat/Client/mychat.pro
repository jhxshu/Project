QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mychat
TEMPLATE = app
RC_ICONS = icon.ico
CONFIG += c++17
DESTDIR = ./bin

SOURCES += \
    chatdialog.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    customizeedit.cpp \
    global.cpp \
    httpmgr.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    tcpmgr.cpp \
    timerbtn.cpp

HEADERS += \
    chatdialog.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    clickedbtn.h \
    clickedlabel.h \
    customizeedit.h \
    global.h \
    httpmgr.h \
    listitembase.h \
    loadingdlg.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h \
    resetdialog.h \
    singleton.h \
    tcpmgr.h \
    timerbtn.h

FORMS += \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    loadingdlg.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetdialog.ui

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug | release) {
    TargetConfig = $${PWD}/config.ini
    TargetConfig = $$replace(TargetConfig, /, \\)
    OutputDir = $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
}
