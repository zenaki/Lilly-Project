#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T08:30:06
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lilly_Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    settings.h \
    global.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settings.ui \
    settingsdialog.ui

RESOURCES += \
    rsc.qrc
