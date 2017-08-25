#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T16:21:21
#
#-------------------------------------------------

QT       += core gui sql network serialport xml
CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = TesterClient
TEMPLATE = app

SOURCES += main.cpp\
    WinSyst.cpp \
    LoginPage.cpp \
    QRCodeTest.cpp \
    UdpSocket.cpp \
    HomePage.cpp \
    MainPage.cpp \
    ConfigPWR.cpp \
    ConfigLoad.cpp \
    ConfPage.cpp \
    TestPage.cpp \
    qcustomplot.cpp \
    ConfigFG.cpp \
    conf_resistance.cpp \
    conf_insulation.cpp \
    conf_current_ac.cpp \
    conf_inductance.cpp \
    ctrl_device_232.cpp \
    page_sqlite.cpp

HEADERS  += \
    WinSyst.h \
    LoginPage.h \
    QRCodeTest.h \
    UdpSocket.h \
    HomePage.h \
    MainPage.h \
    ConfigPWR.h \
    ConfigLoad.h \
    ConfPage.h \
    TestPage.h \
    qcustomplot.h \
    ConfigFG.h \
    conf_resistance.h \
    conf_insulation.h \
    conf_current_ac.h \
    conf_inductance.h \
    conf_delegation.h \
    ctrl_device_232.h \
    page_sqlite.h

FORMS    += \
    WinSyst.ui \
    QRCodeTest.ui

RESOURCES += \
    image.qrc \
    source.qrc

RC_FILE += ico.rc

TRANSLATIONS += myapp.ts

DISTFILES += \
    TesterClient.pro.user
