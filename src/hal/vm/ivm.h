#pragma once
#include "vm/ncDef.h"
#include "printDatas/printDatasDef.h"
#include <QObject>
#include <QQueue>
#include <windows.h>

namespace TIGER_VMSLM
{
    // using namespace TIGER_NCDef;
    class IVM : public QObject
    {
        Q_OBJECT
    public:
        IVM(HWND p_hWnd) : QObject(), m_hWnd(p_hWnd), m_vmStatus(vmsIdle) {};
        virtual ~IVM(){};

        virtual void init() = 0;
        // virtual void jog(CAxisID p_id, double p_len, double p_vel) = 0;
        // virtual void jogTo(CAxisID p_id, double p_pos, double p_vel) = 0;
        // virtual void stop(CAxisID p_id) = 0;
        // virtual void stop() = 0;
        // virtual void zero(CAxisID p_id) = 0;
        // virtual void stopZero(CAxisID p_id) = 0;
        // virtual void stopZero() = 0;
        // virtual void setPos(CAxisID p_id, double p_pos) = 0;
        // virtual void zeroEncoder() = 0;
        // virtual void print(QList<QQueue<TIGER_Print::CFrameData>> p_frames) = 0;
        // virtual void stopPrint() = 0;
        // virtual void endPrint() = 0;
        // virtual void flush(QList<QQueue<TIGER_Print::CFrameData>> p_frames) = 0;
        // virtual void flush() = 0;
        // virtual void stopFlush() =0;
        // virtual void autoFlush() = 0;
        // virtual void stopAutoFlush() =0;
        // virtual void statusBar() = 0;
        // virtual void stopStatusBar() = 0;
        virtual void manu() = 0;
        virtual void stopManu() = 0;
        virtual void manuOnce() = 0;
        virtual void stopManuOnce() = 0;
        virtual void spread() = 0;
        virtual void stopSpread() = 0;
        // virtual void pause() = 0;
        // virtual void resume() = 0;
        // virtual void openDO(unsigned int p_id) = 0;
        // virtual void closeDO(unsigned int p_id) = 0;
        // virtual void clearAlarms() = 0;
        // virtual void loadParas() = 0;
        // virtual int getManuTime() = 0;
        // virtual int getManuStartTime() = 0;
        // virtual const CNCStatus ncStatus() = 0;
        // virtual const QList<CPCStatus> pcStatus() = 0;
        virtual void changeVMStatus(CVMStatus p_status) = 0;

        virtual void startMark() = 0;
        // virtual void pauseMark() = 0;
        // virtual void continueMark() = 0;
        virtual void stopMark() = 0;
        virtual void nativeEvent(MSG* p_message) = 0;
        virtual void GMCStarMark() = 0;
        virtual void creatUdmBin(std::vector<std::vector<TIGER_PrintDatas::lineSegment>> p_segments) = 0;

        virtual bool sendDiscreteInputs(int p_addr, bool p_value) = 0;
        virtual bool sendInputRegisters(int p_addr, bool p_value) = 0;

    signals:
        void sigConnected();
        // void sigDisconnected();

        // void sigAxisStatusUpdate(unsigned int p_id,QDateTime p_timestamp);
        // void sigAxisAlarm(unsigned int p_id);
        // void sigPrintUpdate();
        void sigVMStatusUpdate();
        // void sigDiStatusUpdate();
        // void sigDoStatusUpdate();

        // void sigFlushEnd();
        // void sigAutoFlushEnd();
        void sigManuEnd();
        void sigManuOnceEnd();
        void sigSpreadEnd();
        // void sigStatusBarEnd();
        void sigSliceUpdate();
        // void sigStartPrint();
        // void sigPrintFinished();
        void sigMarkEnd();
        void sigDownloadEnd();
        void sigExecProcess(unsigned int p_pValue);

    protected:
        HWND m_hWnd;  // 窗口句柄
        CVMStatus m_vmStatus;  // 虚拟机状态
        // CNCStatus m_ncStatus;  // 数控状态
        // QList<CPCStatus> m_pcStatus;  // PC状态
    };
}