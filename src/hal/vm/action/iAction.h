#pragma once
#include <QObject>
#include "../vm.h"

namespace TIGER_VMSLM
{
    class CVM;

    class IAction : public QObject
    {
        Q_OBJECT
    public:
        IAction(CVM *p_vm);
        virtual ~IAction();
        virtual void start();
        virtual void stop();

    signals:
        void sigEnd();

    protected:
        CVM *m_pVM;
        bool m_bStop;
        int runCount;
    };


    class CActionCreater
    {
    public:
        CActionCreater(CVM *p_vm);
        IAction *autoWorkAction();

    protected:
        CVM *m_pVM;
    };
}