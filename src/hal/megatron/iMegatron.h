#pragma once
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QQueue>
#include "megatronDef.h"

namespace TIGER_Megatron
{
    class IMegatron : public QObject
    {
        friend class IMegatronController;
        Q_OBJECT
    public:
        IMegatron();
        virtual ~IMegatron();
        void setMegatronIP(CMegatronIP p_ips);
        void connectMegatron();

    protected:
        virtual void init() = 0;

    signals:
        void sigConnect();
        void sigConnected();
        void sigDisconnected();
        void sigError(QString p_error);

    protected slots:
        virtual void slotConnectDevice() = 0;
        virtual void slotConnected() = 0;
        virtual void slotDisconnected() = 0;

    protected:
        CMegatronIP m_ip;
        QString m_error;
        volatile bool m_bConnected;
        uint32_t m_reConnectTimes;
    };

    class IMegatronController : public QObject
    {
        Q_OBJECT
        QThread workerThread;

    public:
        IMegatronController(IMegatron *p_pMegatron)
        {
            p_pMegatron->moveToThread(&workerThread);
            connect(p_pMegatron, &IMegatron::sigConnect, p_pMegatron, &IMegatron::slotConnectDevice);
            connect(p_pMegatron, &IMegatron::sigConnected, p_pMegatron, &IMegatron::slotConnected);
            connect(p_pMegatron, &IMegatron::sigDisconnected, p_pMegatron, &IMegatron::slotDisconnected);
            workerThread.start();
        };
        ~IMegatronController()
        {
            workerThread.quit();
            workerThread.wait();
        };
    };
}