#pragma once
#include "vm/ncDef.h"
#include "printDatas/printDatasDef.h"
#include <QObject>
#include <QQueue>
#include <windows.h>

namespace TIGER_VMSLM
{
    class IVM : public QObject
    {
        Q_OBJECT
    public:
        IVM(HWND p_hWnd) : QObject(), m_hWnd(p_hWnd){};
        virtual ~IVM(){};

        virtual void init() = 0;
        virtual void changeVMWorkStatus(CVMWorkStatus p_status) = 0;
        virtual void changeVMMarkStatus(CVMMarkStatus p_status) = 0;

        virtual void manu() = 0;
        virtual void stopManu() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void manuOnce() = 0;
        virtual void stopManuOnce() = 0;
        virtual void spread() = 0;
        virtual void stopSpread() = 0;
        virtual void startMark() = 0;
        virtual void stopMark() = 0;
        // virtual void clearAlarms() = 0;
        // virtual void loadParas() = 0;
        // virtual int getManuTime() = 0;
        // virtual int getManuStartTime() = 0;

        virtual void nativeEvent(MSG* p_message) = 0;
        virtual void GMCStarMark() = 0;
        virtual void GMCPauseMark() = 0;
        virtual void GMCContinueMark() = 0;
        virtual void GMCStopMark() = 0;
        virtual void creatUdmBin(TIGER_PrintDatas::layerDatas p_layerDatas) = 0;

        virtual bool sendDiscreteInputs(int p_addr, bool p_value) = 0;
        virtual bool sendInputRegisters(int p_addr, bool p_value) = 0;

    signals:
        void sigConnected();
        // void sigDisconnected();

        // void sigPrintUpdate();
        void sigVMStatusUpdate();

        void sigManuEnd();
        void sigManuOnceEnd();
        void sigSpreadEnd();
        void sigSliceUpdate();
        // void sigStartPrint();
        // void sigPrintFinished();
        void sigMarkEnd();
        void sigDownloadEnd();
        void sigExecProcess(unsigned int p_pValue);

    protected:
        HWND m_hWnd;

    public:
        CVMStatusInfo m_vmStatusInfo;
    };
}