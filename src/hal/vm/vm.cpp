#include "vm.h"
#include "ncDef.h"
#include "./action/iAction.h"

namespace TIGER_VMSLM
{
    CVM::CVM()
    {
        m_plcServer = new CModbusServer(this);
        m_plcControl = new IPlcController(m_plcServer);

        auto pActionCreator = new CActionCreater(this);
        m_pAutoWorkAction = pActionCreator->autoWorkAction();
    }

    CVM::~CVM()
    {
    }

    void CVM::init()
    {
        m_plcServer->setPlcIP(CModbusServer::CPlcIP(cnStr("PLC"), plcParas()->ip, plcParas()->ipPort, plcParas()->ipMAC));
        m_plcServer->connectPlc();
    }

    void CVM::autoWork()
    {
        m_pAutoWorkAction->start();
    }

    void CVM::stopWork()
    {
        m_pAutoWorkAction->stop();
    }

    bool CVM::sendDisColis(int p_addr, bool p_value)
    {
        return m_plcServer->sendDisColis(p_addr, p_value);
    }
}