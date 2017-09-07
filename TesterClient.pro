#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T16:21:21
#
#-------------------------------------------------

QT       += core gui sql network serialport xml
CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

linux{
LIBS += /usr/lib/i386-linux-gnu/libqrencode.so
}
win32{
LIBS+= E:\Code\TesterClient\qrcodelib.lib
LIBS+= E:\Code\TesterClient\qrcodelib.dll
}
TARGET = TesterClient
TEMPLATE = app

SOURCES += main.cpp\
    WinSyst.cpp \
    UdpSocket.cpp \
    HomePage.cpp \
    MainPage.cpp \
    ConfPage.cpp \
    TestPage.cpp \
    qcustomplot.cpp \
    conf_resistance.cpp \
    conf_insulation.cpp \
    conf_current_ac.cpp \
    conf_inductance.cpp \
    page_sqlite.cpp \
    conf_noloadtest.cpp \
    conf_loadtesting.cpp \
    LoginPage.cpp \
    conf_backforce.cpp \
    conf_qrencode.cpp \
    conf_hall.cpp \
    wt330.cpp \
    iobrd.cpp \
    servo.cpp \
    mbdkt.cpp \
    Snowflake.cpp

HEADERS  += \
    WinSyst.h \
    UdpSocket.h \
    HomePage.h \
    MainPage.h \
    ConfPage.h \
    TestPage.h \
    qcustomplot.h \
    conf_resistance.h \
    conf_insulation.h \
    conf_current_ac.h \
    conf_inductance.h \
    conf_delegation.h \
    page_sqlite.h \
    conf_noloadtest.h \
    conf_loadtesting.h \
    LoginPage.h \
    conf_backforce.h \
    conf_qrencode.h \
    conf_hall.h \
    wt330.h \
    iobrd.h \
    servo.h \
    mbdkt.h \
    Snowflake.h

FORMS    += \
    WinSyst.ui

RESOURCES += \
    image.qrc \
    source.qrc

RC_FILE += ico.rc

TRANSLATIONS += myapp.ts

DISTFILES += \
    TesterClient.pro.user
