#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T14:40:39
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeApplianceService
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
    libs/xSql/handle.cpp \
    libs/xSql/xSqlConnector.cpp \
    record.cpp \
    pricelist.cpp \
    mainw.cpp \
    editor.cpp \
    service_add.cpp \
    service_deleter.cpp \
    statistic.cpp

HEADERS += \
    libs/xSql/handle.h \
    libs/xSql/xSqlConnector.h \
    record.h \
    pricelist.h \
    mainw.h \
    editor.h \
    service_add.h \
    service_deleter.h \
    statistic.h

FORMS += \
    record.ui \
    pricelist.ui \
    mainwindow.ui \
    editor.ui \
    service_add.ui \
    service_deleter.ui \
    statistic.ui

RESOURCES += res.qrc


win32: LIBS += -L$$PWD/libs/mysql-connector-c-6.1.11-winx64/lib/ -llibmysql

INCLUDEPATH += $$PWD/libs/mysql-connector-c-6.1.11-winx64/include
DEPENDPATH += $$PWD/libs/mysql-connector-c-6.1.11-winx64/include

win32: LIBS += -L$$PWD/libs/mysql-connector-c-6.1.11-win32/lib/ -llibmysql

INCLUDEPATH += $$PWD/libs/mysql-connector-c-6.1.11-win32/include
DEPENDPATH += $$PWD/libs/mysql-connector-c-6.1.11-win32/include
