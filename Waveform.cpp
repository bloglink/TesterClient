/*******************************************************************************
 * Copyright [2016]   <青岛艾普智能仪器有限公司>
 * All rights reserved.
 *
 * version:     2.1.0.170427
 * author:      zhaonanlin
 * brief:       波形处理模块
*******************************************************************************/
#include "Waveform.h"

Waveform::Waveform(QWidget *parent) : QLabel(parent)
{
    isTest = false;
    isItem = false;
    isBlock = false;
    Block0 = 5;
    Block1 = 395;
    for (int i=0; i < MAX_WAVE; i++) {
        WaveBytes.append("0000");
        WaveTests.append("0000");
    }
}

void Waveform::WaveByteShow(QByteArray msg)
{
    WaveByte = msg;
    WaveByteH.clear();
    WaveByteY.clear();
    if (WaveByte.size() <= 400 && WaveByte.size() >= 395) {
        for (int i=0; i < WaveByte.size(); i++)
            WaveByteH.append(quint8(WaveByte.at(i)));
        for (int i=0; i < this->width(); i++) {
            int t = i*WaveByteH.size()/this->width();
            WaveByteY.append(this->height()-WaveByteH.at(t)*this->height()/256);
        }
        this->update();
        return;
    }
    if (WaveByte.size() <= 800 && WaveByte.size() >= 790) {
        for (int i=0; i < WaveByte.size()/2; i++)
            WaveByteH.append(quint16(WaveByte.at(i*2))*256+quint8(WaveByte.at(i*2+1)));
        for (int i=0; i < this->width(); i++) {
            int t = i*WaveByteH.size()/this->width();
            WaveByteY.append(this->height()-WaveByteH.at(t)*this->height()/1024);
        }
        this->update();
        return;
    }
    this->update();
}

void Waveform::WaveTestShow(QByteArray msg)
{
    WaveTest = msg;
    WaveTestH.clear();
    WaveTestY.clear();
    if (WaveTest.size() <= 400 && WaveTest.size() >= 395) {
        for (int i=0; i < WaveTest.size(); i++)
            WaveTestH.append(quint8(WaveTest.at(i)));
        for (int i=0; i < this->width(); i++) {
            int t = i*WaveTestH.size()/this->width();
            WaveTestY.append(this->height()-WaveTestH.at(t)*this->height()/256);
        }
        isTest = true;
        this->update();
        return;
    }
    if (WaveTest.size() <= 800 && WaveTest.size() >= 790) {
        for (int i=0; i < WaveTest.size()/2; i++)
            WaveTestH.append(quint16(WaveTest.at(i*2))*256+quint8(WaveTest.at(i*2+1)));
        for (int i=0; i < this->width(); i++) {
            int t = i*WaveTestH.size()/this->width();
            WaveTestY.append(this->height()-WaveTestH.at(t)*this->height()/1024);
        }
        isTest = true;
        this->update();
        return;
    }
    this->update();
}

void Waveform::WaveItemShow(QByteArray msg)
{
    WaveItem = msg;
    isItem = true;
}

void Waveform::WaveBlock(int b1,  int b2)
{
    Block0 = b1;
    Block1 = b2;
    isBlock = true;
    this->update();
}

void Waveform::InitWaveByte(quint8 s)
{
    QByteArray w = WaveBytes.at(s);
    WaveByteH.clear();
    for (int i=0; i < w.size()/2; i++) {
        WaveByteH.append(quint16(w.at(i*2)*256) + quint8(w.at(i*2+1)));
    }
}

void Waveform::InitWaveTest(quint8 s)
{
    QByteArray w = WaveTests.at(s);
    WaveTestH.clear();
    for (int i=0; i < w.size()/2; i++) {
        WaveTestH.append(quint16(w.at(i*2)*256) + quint8(w.at(i*2+1)));
    }
}

void Waveform::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(QPen(Qt::darkGreen,  1,  Qt::DotLine));
    //横线
    painter->drawLine(QPoint(0, height()*3/4), QPoint(width(), height()*3/4));
    painter->drawLine(QPoint(0, height()*2/4), QPoint(width(), height()*2/4));
    painter->drawLine(QPoint(0, height()*1/4), QPoint(width(), height()*1/4));
    //坚线
    painter->drawLine(QPoint(width()*1/4, 0), QPoint(width()*1/4, height()));
    painter->drawLine(QPoint(width()*2/4, 0), QPoint(width()*2/4, height()));
    painter->drawLine(QPoint(width()*3/4, 0), QPoint(width()*3/4, height()));

    if (isTest) {
        painter->setPen(QPen(Qt::white,  1,  Qt::SolidLine));
        for (int i=0; i < WaveTestY.size()-1; i++) {
            painter->drawLine(QPoint(i, WaveTestY.at(i)), QPoint(i+1, WaveTestY.at(i+1)));
        }
    }

    painter->setPen(QPen(Qt::green,  1,  Qt::SolidLine));
    for (int i=0; i < WaveByteY.size()-1; i++) {
        painter->drawLine(QPoint(i, WaveByteY.at(i)), QPoint(i+1, WaveByteY.at(i+1)));
    }

    if (isItem) {
        painter->drawText(QPoint(width()*3/4, height()*1/4-5), WaveItem);
    }
    if (isBlock) {
        int b0 = Block0*this->width()/400;
        int b1 = Block1*this->width()/400;
        painter->setPen(QPen(Qt::yellow,  1,  Qt::SolidLine));
        painter->drawLine(QPoint(b0, 0), QPoint(b0, height()));
        painter->setPen(QPen(Qt::red,  1,  Qt::SolidLine));
        painter->drawLine(QPoint(b1, 0), QPoint(b1, height()));
    }
    painter->end();
    e->accept();
}

void Waveform::resizeEvent(QResizeEvent *e)
{
    WaveByteShow(WaveByte);
    if (isTest)
        WaveTestShow(WaveTest);
    if (isBlock)
        WaveBlock(Block0, Block1);
    e->accept();
}

void Waveform::mousePressEvent(QMouseEvent *e)
{
    int x = e->x()*400/this->width();
    if (x >= 200)
        Block1 = x;
    else
        Block0 = x;
    this->update();
    emit BlockClick(x);
    QLabel::mousePressEvent(e);
}
/*********************************END OF FILE**********************************/
