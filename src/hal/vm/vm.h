#pragma once
#include "ivm.h"

class CModbusServer;
class IPlcController;
namespace TIGER_Megatron
{
    class CDZSTeGMCMegatron;
    class IMegatronController;
}
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

        virtual void manu() override;
        virtual void stopManu() override;
        virtual void manuOnce() override;
        virtual void stopManuOnce() override;
        virtual void spread() override;
        virtual void stopSpread() override;

        virtual void startMark() override;
        // virtual void pauseMark() override;
        // virtual void continueMark() override;
        virtual void stopMark() override;
        virtual void nativeEvent(MSG* p_message) override;
        virtual void GMCStarMark() override;
        virtual void creatUdmBin(std::vector<std::vector<TIGER_PrintDatas::lineSegment>> p_segments) override;
        virtual void creatUdmBin(TIGER_PrintDatas::layerDatas p_layerDatas) override;

        virtual bool sendDiscreteInputs(int p_addr, bool p_value) override;
        virtual bool sendInputRegisters(int p_addr, bool p_value) override;

    protected:
        CModbusServer *m_plcServer;
        IPlcController *m_plcControl;

        TIGER_Megatron::CDZSTeGMCMegatron *m_megatron;
        TIGER_Megatron::IMegatronController *m_megatronController;

        IAction *m_pManuAction;
        IAction *m_pManuOnceAction;
        IAction *m_pMarkAction;
        IAction *m_pSpreadAction;
    };
}