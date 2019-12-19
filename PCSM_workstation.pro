#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T14:40:39
#
#-------------------------------------------------

QT       += core gui sql network

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

INCLUDEPATH += x:/libs/cpp/boost/v1.70/

SOURCES += \
  comp_deleter.cpp \
  comp_type_add.cpp \
  comp_type_deleter.cpp \
  customer_add.cpp \
  customer_deleter.cpp \
  customers.cpp \
  employee_add.cpp \
  employee_deleter.cpp \
  employees.cpp \
  export.cpp \
  libs/ODBCConnector/ODBCW.cpp \
  libs/ODBCConnector/query_controller.cpp \
        main.cpp \
  order_add.cpp \
  order_deleter.cpp \
    pricelist.cpp \
    mainw.cpp \
    comp_add.cpp \
    statistic.cpp

HEADERS += \
    PCSM_base_types.h \
    comp_deleter.h \
    comp_type_add.h \
    comp_type_deleter.h \
    component.h \
    customer_add.h \
    customer_deleter.h \
    customers.h \
    employee_add.h \
    employee_deleter.h \
    employees.h \
    export.h \
    libs/ODBCConnector/ODBCW.h \
    libs/ODBCConnector/query_controller.h \
    order_add.h \
    order_deleter.h \
    pricelist.h \
    mainw.h \
    comp_add.h \
    statistic.h

FORMS += \
    comp_deleter.ui \
    comp_type_add.ui \
    comp_type_deleter.ui \
    customer_add.ui \
    customer_deleter.ui \
    customers.ui \
    employee_add.ui \
    employee_deleter.ui \
    employees.ui \
    export.ui \
    order_add.ui \
    order_deleter.ui \
    pricelist.ui \
    mainwindow.ui \
    comp_add.ui \
    statistic.ui

RESOURCES += res.qrc
