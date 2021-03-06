#-------------------------------------------------
#
# Project created by QtCreator 2019-01-22T23:49:39
#
#-------------------------------------------------

QT       += core gui serialbus serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NTREX_Autonomous
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
        main.cpp \
        mainwindow.cpp \
    engine.cpp \
    Communication/canmanager.cpp \
    canconnectdialog.cpp \
    dialog.cpp \
    bitratebox.cpp \
    sendframebox.cpp \
    thread.cpp \
    worker.cpp \
    canthread.cpp \
    canmonitordialog.cpp \
    StateMachine/states.cpp \
    manualcontroldialog.cpp \
    steercontroller.cpp \
    speedmanager.cpp \
    steermanager.cpp \
    Controller/pidcontroller.cpp \
    Common/common.cpp \
    serialconnectdialog.cpp \
    Communication/serialmanager.cpp \
    serialmonitordialog.cpp





HEADERS += \
        mainwindow.h \
    engine.h \
    Communication/canmanager.h \
    dialog.h \
    bitratebox.h \
    sendframebox.h \
    thread.h \
    canconnectdialog.h \
    worker.h \
    canthread.h \
    canmonitordialog.h \
    StateMachine/states.h \
    manualcontroldialog.h \
    steercontroller.h \
    speedmanager.h \
    steermanager.h \
    Controller/pidcontroller.h \
    Common/common.h \
    serialconnectdialog.h \
    Communication/serialmanager.h \
    serialmonitordialog.h

RESOURCES += image.qrc

FORMS += \
        mainwindow.ui \
    canconnectdialog.ui \
    dialog.ui \
    sendframebox.ui \
    cancommwidget.ui \
    canmonitordialog.ui \
    manualcontroldialog.ui \
    serialconnectdialog.ui \
    serialmonitordialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
