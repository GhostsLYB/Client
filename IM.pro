#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T11:30:31
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        allpagelistwidget.cpp \
        chatpage.cpp \
        customUI/customlistwidgetitem.cpp \
        customUI/friendlistitem.cpp \
        customUI/homepagemenu.cpp \
        customUI/homepagetop.cpp \
        customWidget/widgetfriend.cpp \
        customWidget/widgetmain.cpp \
        database/sqlitecontrol.cpp \
        homepage.cpp \
        main.cpp \
        login.cpp \
        register.cpp \
        socket/control.cpp \
        socket/socketcontrol.cpp \
        userinfopage.cpp

HEADERS += \
        allpagelistwidget.h \
        chatpage.h \
        customUI/customlistwidgetitem.h \
        customUI/friendlistitem.h \
        customUI/homepagemenu.h \
        customUI/homepagetop.h \
        customWidget/widgetfriend.h \
        customWidget/widgetmain.h \
        database/sqlitecontrol.h \
        homepage.h \
        login.h \
        register.h \
        socket/control.h \
        socket/socketcontrol.h \
        userinfopage.h

FORMS += \
        allpagelistwidget.ui \
        chatpage.ui \
        customUI/customlistwidgetitem.ui \
        customUI/friendlistitem.ui \
        customUI/homepagemenu.ui \
        customUI/homepagetop.ui \
        customWidget/widgetfriend.ui \
        customWidget/widgetmain.ui \
        homepage.ui \
        login.ui \
        register.ui \
        userinfopage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    readme.txt
