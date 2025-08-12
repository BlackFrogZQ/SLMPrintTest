#include "vm.h"
#include "ncDef.h"
#include "plc/plcSigDef.h"
#include "plc/modbusServer.h"
#include "megatron/megatronDef.h"
#include "megatron/printMegatron/dzSTeGMCMegatron.h"
#include "./action/iAction.h"
#include "./action/iActionState.h"

using namespace std;
using namespace TIGER_Megatron;
using namespace TIGER_PrintDatas;
namespace TIGER_VMSLM
{
    CVM::CVM(HWND p_hWnd) : IVM(p_hWnd), m_megatron(nullptr)
    {
        m_plcServer = new CModbusServer(this);
        m_plcControl = new IPlcController(m_plcServer);
        assert(m_plcServer != nullptr);
        assert(m_plcControl != nullptr);
        connect(m_plcServer, &CModbusServer::sigConnected, this, [this]()
        {
            m_vmStatusInfo.GMCConnected = true;
        });
        connect(m_plcServer, &CModbusServer::sigDisconnected, this, [this]()
        {
            m_vmStatusInfo.GMCConnected = false;
        });

        m_megatron = new CDZSTeGMCMegatron(m_hWnd);
        m_megatronController = new IMegatronController(m_megatron);
        assert(m_megatron != nullptr);
        assert(m_megatronController != nullptr);
        connect(m_megatron, &CDZSTeGMCMegatron::sigConnected, this, [this]()
        {
            m_vmStatusInfo.plcConnected = true;
        });
        connect(m_megatron, &CDZSTeGMCMegatron::sigDisconnected, this, [this]()
        {
            m_vmStatusInfo.plcConnected = false;
        });
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
            if (m_vmStatusInfo.vmWorkStatus == vmsStartManu)
            {
                changeVMWorkStatus(vmsIdle);
            }
            emit sigManuEnd();
        });
        connect(m_pManuOnceAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatusInfo.vmWorkStatus == vmsManuOnce)
            {
                changeVMWorkStatus(vmsIdle);
            }
            emit sigManuOnceEnd();
        });
        connect(m_pSpreadAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatusInfo.vmWorkStatus == vmsSpread)
            {
                changeVMWorkStatus(vmsIdle);
            }
            emit sigSpreadEnd();
        });
        connect(m_pMarkAction, &IAction::sigEnd, this, [this]()
        {
            if (m_vmStatusInfo.vmWorkStatus == vmsMark)
            {
                changeVMWorkStatus(vmsIdle);
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

        m_megatron->setMegatronIP(CMegatronIP(cnStr("振镜控制卡"), MarkConnectPara()->ip));
        m_megatron->connectMegatron();
    }

    void CVM::manu()
    {
        m_pManuAction->start();
        changeVMWorkStatus(vmsStartManu);
    }

    void CVM::stopManu()
    {
        m_pManuAction->stop();
        changeVMWorkStatus(vmsIdle);
    }

    void CVM::pause()
    {
        // m_pManuAction->pause();
        changeVMWorkStatus(vmsPauseManu);
    }

    void CVM::resume()
    {
        // m_pManuAction->resume();
        changeVMWorkStatus(vmsStartManu);
    }

    void CVM::manuOnce()
    {
        m_pManuOnceAction->start();
        changeVMWorkStatus(vmsManuOnce);
    }

    void CVM::stopManuOnce()
    {
        m_pManuOnceAction->stop();
        changeVMWorkStatus(vmsIdle);
    }

    void CVM::spread()
    {
        m_pSpreadAction->start();
        changeVMWorkStatus(vmsSpread);
    }

    void CVM::stopSpread()
    {
        assert(m_vmStatusInfo.vmWorkStatus == vmsSpread);
        m_pSpreadAction->stop();
        changeVMWorkStatus(vmsIdle);
    }

    void CVM::startMark()
    {
        m_pMarkAction->start();
        changeVMWorkStatus(vmsMark);
    }

    void CVM::stopMark()
    {
        assert(m_vmStatusInfo.vmWorkStatus == vmsMark);
        m_pMarkAction->stop();
        changeVMWorkStatus(vmsIdle);
    }

    void CVM::nativeEvent(MSG* p_message)
    {
        assert(p_message != nullptr);
        if(m_megatron)
        {
            m_megatron->nativeEvent(p_message);
        }
    }

    void CVM::creatUdmBin(layerDatas p_layerDatas)
    {
        m_megatron->creatUdmBin(p_layerDatas);
    }

    void CVM::GMCStarMark()
    {
        m_megatron->starMark();
        changeVMMarkStatus(cmsStartMark);
    }

    void CVM::GMCPauseMark()
    {
        m_megatron->PauseMark();
        changeVMMarkStatus(cmsPauseMark);
    }

    void CVM::GMCContinueMark()
    {
        m_megatron->ContinueMark();
        changeVMMarkStatus(cmsContinueMark);
    }

    void CVM::GMCStopMark()
    {
        m_megatron->StopMark();
        changeVMMarkStatus(cmsStopMark);
    }

    bool CVM::sendDiscreteInputs(int p_addr, bool p_value)
    {
        return m_plcServer->sendDiscreteInputs(p_addr, p_value);
    }

    bool CVM::sendInputRegisters(int p_addr, bool p_value)
    {
        return m_plcServer->sendInputRegisters(p_addr, p_value);
    }

    void CVM::changeVMWorkStatus(CVMWorkStatus p_status)
    {
        if (m_vmStatusInfo.vmWorkStatus == p_status)
        {
            return;
        }
        m_vmStatusInfo.vmWorkStatus = p_status;
        myInfo << cnStr("虚拟机工作状态为: %1").arg(VMWorkStatusCN[p_status]);
        emit sigVMStatusUpdate();
    }

    void CVM::changeVMMarkStatus(CVMMarkStatus p_status)
    {
        if (m_vmStatusInfo.vmMarkStatus == p_status)
        {
            return;
        }
        m_vmStatusInfo.vmMarkStatus = p_status;
        emit sigVMStatusUpdate();
    }
}