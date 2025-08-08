#pragma once
#include "HM_HashuScan.h"
#include "HM_HashuUDM.h"
#include "hal/vm/ncDef.h"
#include "../iMegatron.h"
#include "../megatronDef.h"
#include <windows.h>

namespace TIGER_Megatron
{
    class CDZSTeGMCMegatron : public IMegatron
    {
        Q_OBJECT
    public:
        CDZSTeGMCMegatron(HWND p_hWnd);
        ~CDZSTeGMCMegatron();

        void starMark();
        void PauseMark();
        void ContinueMark();
        void StopMark();
        void nativeEvent(MSG* p_message);
        void creatUdmBin(TIGER_PrintDatas::layerDatas p_layerDatas);

    protected:
        LRESULT OnMsgDeviceEhco(WPARAM wParam, LPARAM lParam);
        LRESULT OnMsgUDMDownloadEnd(WPARAM wParam, LPARAM lParam);
        LRESULT OnMsgUDMRunHalt(WPARAM wParam, LPARAM lParam);
        LRESULT ExecProcess(WPARAM wParam, LPARAM lParam);
        MarkParameter* getMarkParameter();
        void downloadMarkFile();
        void printError(const QString &p_msg);
        bool checkErrorCode(int errorCode, QString preMsg = QString());

    protected:
        virtual void init() override;
        static QString formatErrorMsg(QString p_error, GMC_STATUS errorCode);

    protected slots:
        virtual void slotConnectDevice() override;
        virtual void slotConnected() override;
        virtual void slotDisconnected() override;

    signals:
        void sigExecProcess(unsigned int p_pValue);
        void sigDownloadEnd();
        void sigMarkEnd();

    private:
        HWND m_hWnd;
        MarkParameter *m_pMarkParameter;
        int m_ipIndex;
        int pErrorCode;
    };

    struct CDZSTeGMCState
    {
    public:
        void setDownloadStatus(bool p_isDownload) { m_bIsDownload = p_isDownload; }
        void setMarkingStatus(bool p_isMarking) { m_bIsMarking = p_isMarking; }
        bool getDownloadStatus() const { return m_bIsDownload; }
        bool getMarkingStatus() const { return m_bIsMarking; }

    private:
        bool m_bIsDownload = false; // 是否下载打标文件
        bool m_bIsMarking = false;  // 是否正在打标
    };
    CDZSTeGMCState *GMCState();
}