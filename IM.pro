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
        control.cpp \
        customlistwidgetitem.cpp \
        homepage.cpp \
        homepagemenu.cpp \
        homepagetop.cpp \
        main.cpp \
        login.cpp \
        register.cpp \
        socketcontrol.cpp \
        sqlitecontrol.cpp \
        widgetfriend.cpp \
        widgetmain.cpp

HEADERS += \
        allpagelistwidget.h \
        chatpage.h \
        control.h \
        customlistwidgetitem.h \
        homepage.h \
        homepagemenu.h \
        homepagetop.h \
        login.h \
        register.h \
        socketcontrol.h \
        sqlitecontrol.h \
        widgetfriend.h \
        widgetmain.h

FORMS += \
        allpagelistwidget.ui \
        chatpage.ui \
        customlistwidgetitem.ui \
        homepage.ui \
        homepagemenu.ui \
        homepagetop.ui \
        login.ui \
        register.ui \
        widgetfriend.ui \
        widgetmain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    readme.txt
