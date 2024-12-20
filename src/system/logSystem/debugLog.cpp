#include "../../ui/mainWindow.h"
#include "../systemService.h"
#include <QString>
#include <QDateTime>
#include <iostream>

void logOutput(QtMsgType p_type, const QMessageLogContext& p_context, const QString& p_msg)
{
    QString msgType("");
    QString msg = p_msg;
    msg.replace('\"', "");
    QString wirteMsg = QString::fromLocal8Bit("[%1]:%2").arg(QDateTime::currentDateTime().toString("hh.mm.ss")).arg(msg);
    switch (p_type)
    {
    case QtInfoMsg:
        msgType = QString("Info");
        if (mainWindow() != nullptr)
        {
            mainWindow()->printMsg(msg);
            msg.clear();
        }
        break;
    case QtDebugMsg:
        msgType = QString("Debug");
        break;
    case QtWarningMsg:
        msgType = QString("Warning");
        break;
    case QtCriticalMsg:
        msgType = QString("Critical");
        break;
    case QtFatalMsg:
        msgType = QString("Fatal");
        break;
    default:
        msgType = QString("Unknown Type");
        break;
    }
    wirteMsg = QString::fromLocal8Bit("%1��Ϣ--%2,\nfile:%3,line:%4").arg(msgType, 12, ' ').arg(wirteMsg).arg(p_context.file).arg(p_context.line);
    // ���������Ϣ��ʽ
    std::cout << wirteMsg.toLocal8Bit().data() << std::endl;
};