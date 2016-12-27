#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T08:30:06
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lilly_Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    init_mysql.cpp \
    logindialog.cpp \
    formkeluarga.cpp

HEADERS  += mainwindow.h \
    global.h \
    settingsdialog.h \
    init_mysql.h \
    logindialog.h \
    formkeluarga.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    logindialog.ui \
    formkeluarga.ui

RESOURCES += \
    rsc.qrc
