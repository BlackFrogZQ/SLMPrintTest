#pragma once
#include "ivm.h"
#include "plc/plcSigDef.h"
#include "plc/modbusServer.h"
#include "./action/iActionState.h"
#include "./action/iAction.h"

namespace TIGER_VMSLM
{
    class IAction;
    class CVM : public IVM
    {
        Q_OBJECT
    public:
        CVM();
        ~CVM();

        virtual void init() override;
        virtual void autoWork() override;
        virtual void stopWork() override;
        virtual bool sendDisColis(int p_addr, bool p_value) override;

    protected:
        CModbusServer *m_plcServer;
        IPlcController *m_plcControl;

        IAction *m_pAutoWorkAction;
    };
}