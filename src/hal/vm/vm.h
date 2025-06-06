#pragma once
#include "ivm.h"
#include "plc/plcSigDef.h"
#include "plc/modbusServer.h"
#include "megatron/megatronDef.h"
#include "megatron/printMegatron/dzSTeGMCMegatron.h"
#include "./action/iActionState.h"
#include "./action/iAction.h"

using namespace TIGER_Megatron;

namespace TIGER_VMSLM
{
    class IAction;
    class CVM : public IVM
    {
        Q_OBJECT
    public:
        CVM(HWND p_hWnd = nullptr);
        ~CVM();

        virtual void init() override;
        virtual void changeVMStatus(CVMStatus p_status) override;

        virtual void autoWork() override;
        virtual void stopWork() override;
        virtual void startSpread() override;
        virtual void stopSpread() override;

        virtual void startMark() override;
        virtual void pauseMark() override;
        virtual void continueMark() override;
        virtual void stopMark() override;
        virtual void nativeEvent(MSG* p_message) override;
        virtual void creatUdmBin(std::vector<std::vector<TIGER_PrintDatas::Segment>> p_segments) override;

        virtual bool sendDisColis(int p_addr, bool p_value) override;
        virtual bool sendHold(int p_addr, bool p_value) override;

    protected:
        CModbusServer *m_plcServer;
        IPlcController *m_plcControl;

        CDZSTeGMCMegatron *m_megatron;
        IMegatronController *m_megatronController;

        IAction *m_pManuAction;
        IAction *m_pMarkAction;
        IAction *m_pSpreadAction;
    };
}