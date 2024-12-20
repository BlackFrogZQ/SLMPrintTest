#pragma once
#include <QDialog>
#include <QDebug>

#define iToStr(num) QString::number(num)
#define dToStr(num,decimals) QString::number(num,'f',decimals)
#define cnStr(str) QString::fromLocal8Bit(str)
#define cnHStr(str) QString::fromLocal8Bit(str).toStdString().data()
#define qToHStr(str) (str).toStdString().data()
#define hToQStr(str) (str).S().Text()
#define cToHStr(str) (str).toStdString().c_str()

#define myDebug qDebug().noquote()
#define myInfo qInfo().noquote()

#ifdef Trace
#define myTrace(qDebugMsg) myDebug << "myTrace:" << qDebugMsg
#else
#define myTrace(qDebugMsg)
#endif

#ifdef Trace
#define trace(qDebugMsg) qDebug() << "trace:" << qDebugMsg
#else
#define trace(qDebugMsg)
#endif

#define delPtr(ptr)     \
    if (ptr != nullptr) \
    {                   \
        delete ptr;     \
        ptr = nullptr;  \
    }