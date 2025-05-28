#pragma once
#include "ivm.h"
#include "plc/plcSigDef.h"
#include "plc/modbusServer.h"

namespace TIGER_VMSLM
{
    class CVM : public IVM
    {
        Q_OBJECT
    public:
        CVM();
        ~CVM();
        void init();

    protected:
        CModbusServer *m_plcServer;
        IPlcController *m_plcControl;
    };
}