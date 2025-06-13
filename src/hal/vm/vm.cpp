#include "vm.h"
#include "ncDef.h"
#include "./action/iAction.h"

namespace TIGER_VMSLM
{
    CVM::CVM(HWND p_hWnd) : IVM(p_hWnd), m_megatron(nullptr)
    {
        m_plcServer = new CModbusServer(this);
        m_plcControl = new IPlcController(m_plcServer);
        assert(m_plcServer != nullptr);
        assert(m_plcControl != nullptr);

        m_megatron = new CDZSTeGMCMegatron(m_hWnd);
        m_megatronController = new IMegatronController(m_megatron);
        assert(m_megatron != nullptr);
        assert(m_megatronController != nullptr);
        connect(m_megatron, &CDZSTeGMCMegatron::sigMarkEnd, this, [this]()
        {
            emit sigMarkEnd();
        });
        connect(m_megatron, &CDZSTeGMCMegatron::sigDownloadEnd, this, [this]()
        {
            emit sigDownloadEnd();
        });
        connect(m_megatron, &CDZSTeGMCMegatron::sigExecProcess, this, [this](unsigned int p_value)
        {
            emit sigExecProcess(p_value);
        });
        connect(m_megatron, &CDZSTeGMCMegatron::sigError, this, [this](const QString &p_msg)
        {
            myInfo << p_msg;
        });

        auto pActionCreator = new CActionCreater(this);
        assert(pActionCreator != nullptr);
        m_pManuAction = pActionCreator->manuAction();
        m_pManuOnceAction = pActionCreator->manuOnceAction();
        m_pMarkAction = pActionCreator->markAction();
        m_pSpreadAction = pActionCreator->spreadAction();
        delete pActionCreator;
        pActionCreator = nullptr;
        connect(m_pManuAction, &IAction::sigSliceUpdate, this, &CVM::sigSliceUpdate);
        connect(m_pManuAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatus == vmsManu)
            {
                changeVMStatus(vmsIdle);
            }
            emit sigManuEnd();
        });
        connect(m_pManuOnceAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatus == vmsManuOnce)
            {
                changeVMStatus(vmsIdle);
            }
            emit sigManuOnceEnd();
        });
        connect(m_pSpreadAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatus == vmsSpread)
            {
                changeVMStatus(vmsIdle);
            }
            emit sigSpreadEnd();
        });
        connect(m_pMarkAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatus == vmsMark)
            {
                changeVMStatus(vmsIdle);
            }
            emit sigMarkEnd();
        });
    }

    CVM::~CVM()
    {
        delPtr(m_megatron);
    }

    void CVM::init()
    {
        m_plcServer->setPlcIP(CModbusServer::CPlcIP(cnStr("PLC"), plcParas()->ip, plcParas()->ipPort, plcParas()->ipMAC));
        m_plcServer->connectPlc();

        m_megatron->setMegatronIP(CMegatronIP(cnStr("振镜控制卡"), DZSTMarkConnectPara()->ip));
        m_megatron->connectMegatron();
    }

    void CVM::manu()
    {
        m_pManuAction->start();
    }

    void CVM::stopManu()
    {
        m_pManuAction->stop();
    }

    void CVM::manuOnce()
    {
        m_pManuOnceAction->start();
    }

    void CVM::stopManuOnce()
    {
        m_pManuOnceAction->stop();
    }

    void CVM::spread()
    {
        m_pSpreadAction->start();
        changeVMStatus(vmsSpread);
    }

    void CVM::stopSpread()
    {
        assert(m_vmStatus == vmsSpread);
        m_pSpreadAction->stop();
        changeVMStatus(vmsIdle);
    }

    void CVM::startMark()
    {
        m_pMarkAction->start();
        changeVMStatus(vmsMark);
    }

    void CVM::stopMark()
    {
        assert(m_vmStatus == vmsMark);
        m_pMarkAction->stop();
        changeVMStatus(vmsIdle);
    }

    void CVM::nativeEvent(MSG* p_message)
    {
        assert(p_message != nullptr);
        if(m_megatron)
        {
            m_megatron->nativeEvent(p_message);
        }
    }

    void CVM::creatUdmBin(std::vector<std::vector<TIGER_PrintDatas::lineSegment>> p_segments)
    {
        m_megatron->creatUdmBin(p_segments);
    }

    void CVM::GMCStarMark()
    {
        m_megatron->starMark();
    }

    bool CVM::sendDiscreteInputs(int p_addr, bool p_value)
    {
        return m_plcServer->sendDiscreteInputs(p_addr, p_value);
    }

    bool CVM::sendInputRegisters(int p_addr, bool p_value)
    {
        return m_plcServer->sendInputRegisters(p_addr, p_value);
    }

    void CVM::changeVMStatus(CVMStatus p_status)
    {
        if (m_vmStatus == p_status)
        {
            return;
        }
        m_vmStatus = p_status;
        myInfo << cnStr("虚拟机状态改变为: %1").arg(cvmStatusCn[p_status]);
        emit sigVMStatusUpdate();
    }
}