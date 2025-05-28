#include "vm.h"

namespace TIGER_VMSLM
{
    CVM::CVM()
    {
        m_plcServer = new CModbusServer(this);
        m_plcControl = new IPlcController(m_plcServer);
    }

    CVM::~CVM()
    {
    }

    void CVM::init()
    {
    }
}