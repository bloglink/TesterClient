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
    Histogram.cpp \
    Waveform.cpp \
    PageKey.cpp \
    WinSyst.cpp \
    WinData.cpp \
    PageDcr.cpp \
    PageMag.cpp \
    PageInr.cpp \
    PageAcw.cpp \
    PageImp.cpp \
    PageNum.cpp \
    PageInd.cpp \
    PagePwr.cpp \
    PageLvs.cpp \
    PageLck.cpp \
    PageOut.cpp \
    TcpClient.cpp \
    UdpClient.cpp \
    SqlClient.cpp \
    SerialPort.cpp \
    MessageBox.cpp \
    PageAmp.cpp \
    LoginPage.cpp \
    QRCodeTest.cpp \
    SqlTableModel.cpp \
    UdpSocket.cpp \
    HomePage.cpp \
    AboutPage.cpp \
    MainPage.cpp \
    ConfigDCR.cpp \
    ConfigACW.cpp \
    ConfigIR.cpp \
    ConfigIND.cpp \
    ConfigPWR.cpp \
    ConfigLoad.cpp \
    ConfPage.cpp \
    TestPage.cpp \
    qcustomplot.cpp \
    ConfigFG.cpp \
    PLCPage.cpp

HEADERS  += \
    Histogram.h \
    Waveform.h \
    PageKey.h \
    WinSyst.h \
    WinData.h \
    PageDcr.h \
    PageMag.h \
    PageInr.h \
    PageAcw.h \
    PageImp.h \
    PageNum.h \
    PageInd.h \
    PagePwr.h \
    PageLvs.h \
    PageLck.h \
    PageOut.h \
    TcpClient.h \
    UdpClient.h \
    SqlClient.h \
    SerialPort.h \
    MessageBox.h \
    PageAmp.h \
    define.h \
    LoginPage.h \
    QRCodeTest.h \
    SqlTableModel.h \
    UdpSocket.h \
    HomePage.h \
    AboutPage.h \
    MainPage.h \
    delegate.h \
    ConfigDCR.h \
    ConfigACW.h \
    ConfigIR.h \
    ConfigIND.h \
    ConfigPWR.h \
    ConfigLoad.h \
    ConfPage.h \
    TestPage.h \
    qcustomplot.h \
    ConfigFG.h \
    PLCPage.h

FORMS    += \
    PageKey.ui \
    WinSyst.ui \
    WinData.ui \
    PageDcr.ui \
    PageMag.ui \
    PageInr.ui \
    PageAcw.ui \
    PageImp.ui \
    PageNum.ui \
    PageInd.ui \
    PagePwr.ui \
    PageLvs.ui \
    PageLck.ui \
    PageOut.ui \
    QRCodeTest.ui

RESOURCES += \
    image.qrc \
    source.qrc

RC_FILE += ico.rc

TRANSLATIONS += myapp.ts

DISTFILES += \
    TesterClient.pro.user
