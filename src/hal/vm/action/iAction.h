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
        void sigSliceUpdate();

    protected:
        CVM *m_pVM;
        bool m_bStop;
    };


    class CActionCreater
    {
    public:
        CActionCreater(CVM *p_vm);
        IAction *manuAction();
        IAction *manuOnceAction();
        IAction *markAction();
        IAction *spreadAction();

    protected:
        CVM *m_pVM;
    };
}