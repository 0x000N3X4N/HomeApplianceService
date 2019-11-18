#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T14:40:39
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaptopMaintenanceService
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

INCLUDEPATH += x:/libs/cpp/boost/v1.71/

SOURCES += \
  comp_type_add.cpp \
  libs/ODBCConnector/ODBCW.cpp \
  libs/ODBCConnector/query_controller.cpp \
        main.cpp \
    record.cpp \
    pricelist.cpp \
    mainw.cpp \
    editor.cpp \
    comp_add.cpp \
    service_deleter.cpp \
    statistic.cpp

HEADERS += \
    comp_type_add.h \
    libs/ODBCConnector/ODBCW.h \
    libs/ODBCConnector/query_controller.h \
    record.h \
    pricelist.h \
    mainw.h \
    editor.h \
    comp_add.h \
    service_deleter.h \
    statistic.h

FORMS += \
    comp_deleter.ui \
    comp_type_add.ui \
    record.ui \
    pricelist.ui \
    mainwindow.ui \
    editor.ui \
    comp_add.ui \
    statistic.ui

RESOURCES += res.qrc
