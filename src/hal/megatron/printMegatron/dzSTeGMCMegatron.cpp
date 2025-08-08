#include "dzSTeGMCMegatron.h"
#include "vm/ncDef.h"
#include "system/basic.h"
#include <QTimer>

using namespace TIGER_VMSLM;
using namespace TIGER_PrintDatas;
namespace TIGER_Megatron
{
    QString CDZSTeGMCMegatron::formatErrorMsg(QString p_error, GMC_STATUS errorCode)
    {
        QString errorMsg;
        switch (errorCode)
        {
            case HM_OK:
                errorMsg = p_error + cnStr("成功");
            case HM_FAILED:
                errorMsg = p_error + cnStr("失败");
            case HM_UNKNOWN:
                errorMsg = p_error + cnStr("未知错误");
            default:
                errorMsg = p_error + cnStr("非SDK错误代码") + QString::number(errorCode);
        }
        return errorMsg;
    }
}

namespace TIGER_Megatron
{
    CDZSTeGMCMegatron::CDZSTeGMCMegatron(HWND p_hWnd): m_hWnd(p_hWnd), m_ipIndex(0), pErrorCode(HM_OK)
    {
        m_pMarkParameter = new MarkParameter[mftMax];
        init();
    }

    CDZSTeGMCMegatron::~CDZSTeGMCMegatron()
    {
        delPtr(m_pMarkParameter);
    }

    void CDZSTeGMCMegatron::init()
    {
        checkErrorCode(HM_InitBoard(m_hWnd), cnStr("初始化振镜控制卡"));
    }

    void CDZSTeGMCMegatron::nativeEvent(MSG* p_message)
    {
        assert(p_message != nullptr);
        try
        {
            switch (p_message->message)
            {
                case HM_MSG_DeviceStatusUpdate:
                {
                    OnMsgDeviceEhco(p_message->wParam, p_message->lParam);
                    break;
                }
                case HM_MSG_StreamEnd:
                {
                    OnMsgUDMDownloadEnd(p_message->wParam, p_message->lParam);
                    break;
                }
                case HM_MSG_UDMHalt:
                {
                    OnMsgUDMRunHalt(p_message->wParam, p_message->lParam);
                    break;
                }
                case HM_MSG_ExecProcess:
                {
                    ExecProcess(p_message->wParam, p_message->lParam);
                    break;
                }
                default:
                    break;
            }
        }
        catch (const std::exception& e)
        {
            myInfo << cnStr("CDZSTeGMCMegatron::nativeEvent异常：%1").arg(e.what());
        }
        catch (...)
        {
            myInfo << cnStr("CDZSTeGMCMegatron::nativeEvent未知异常");
        }
    }

    LRESULT CDZSTeGMCMegatron::OnMsgDeviceEhco(WPARAM wParam, LPARAM lParam)
    {
        QString strIP;
        byte2* ipArr = (byte2*)&wParam;
        strIP = QString("%1.%2.%3.%4").arg(ipArr[0]).arg(ipArr[1]).arg(ipArr[2]).arg(ipArr[3]);
        switch (HM_GetConnectStatus(lParam))
        {
            case HM_DEV_Connect:
            {
                m_ipIndex = lParam;
                m_bConnected = true;
                printError(cnStr("%1：设备已经成功连接").arg(strIP));
            }
                break;
            case HM_DEV_Ready:
            {
                m_ipIndex = lParam;
                m_bConnected = false;
                emit sigDisconnected();
                printError(cnStr("%1：设备处于待机状态（已经找到设备IP，可以进行连接）").arg(strIP));
            }
                break;
            case HM_DEV_NotAvailable:
            {
                m_bConnected = false;
                emit sigDisconnected();
                printError(cnStr("%1：未找到设备，控制卡断电或者网线未连接").arg(strIP));
            }
                break;
            default:
                printError(cnStr("%1：未知设备状态").arg(strIP));
                break;
        }
        return 0;
    }

    LRESULT CDZSTeGMCMegatron::OnMsgUDMDownloadEnd(WPARAM wParam, LPARAM lParam)
    {
        GMCState()->setDownloadStatus(false);
        printError(cnStr("UDM下载完成"));
        emit sigDownloadEnd();
        return 0;
    }

    LRESULT CDZSTeGMCMegatron::OnMsgUDMRunHalt(WPARAM wParam, LPARAM lParam)
    {
        GMCState()->setMarkingStatus(false);
        // printError(cnStr("打标完成"));
        emit sigMarkEnd();
        return 0;
    }

    LRESULT CDZSTeGMCMegatron::ExecProcess(WPARAM wParam, LPARAM lParam)
    {
        unsigned int pValue = wParam;
        emit sigExecProcess(pValue);
        return 0;
    }

    void CDZSTeGMCMegatron::starMark()
    {
        checkErrorCode(HM_StartMark(m_ipIndex), cnStr("开始打标"));
    }

    void CDZSTeGMCMegatron::PauseMark()
    {
        checkErrorCode(HM_PauseMark(m_ipIndex), cnStr("暂停打标"));
    }

    void CDZSTeGMCMegatron::ContinueMark()
    {
        checkErrorCode(HM_ContinueMark(m_ipIndex), cnStr("继续打标"));
    }

    void CDZSTeGMCMegatron::StopMark()
    {
        checkErrorCode(HM_StopMark(m_ipIndex), cnStr("停止打标"));
    }

    void CDZSTeGMCMegatron::creatUdmBin(layerDatas p_layerDatas)
    {
        UDM_NewFile();
        UDM_Main();
        UDM_SetProtocol(MarkConnectPara()->MarkProtocol, Mark2D);
        UDM_SetLayersPara(getMarkParameter(), cmtMax);
        int startAddress = UDM_RepeatStart(RepeatMarkCount);

        for (const auto& block : p_layerDatas.pScanBlocks)
        {
            for (const auto& line : block.scanLines)
            {
                vector<structUdmPos> udmLine;
                for (const auto& pt : line.points)
                {
                    structUdmPos pos = {pt.x, pt.y, pt.z, pt.a};
                    udmLine.push_back(pos);
                }

                UDM_AddPolyline2D(udmLine.data(), udmLine.size(), line.pMarkType);
            }
        }

        UDM_RepeatEnd(startAddress);
        UDM_Jump(0, 0, 0);
        UDM_EndMain();
        UDM_SaveToFile((char*)"./UDM.bin");
        downloadMarkFile();
    }

    MarkParameter* CDZSTeGMCMegatron::getMarkParameter()
    {
        MarkParameter *m_pMarkParameter = new MarkParameter[cmtMax];

        for (size_t i = 0; i < cmtMax; i++)
        {
            const CGalvoMotorParas& motor = getMarkDatas()->markParas[i].motorParas;
            const CLaserParas& laser = getMarkDatas()->markParas[i].laserParas;

            m_pMarkParameter[i].MarkSpeed = motor.MarkSpeed;
            m_pMarkParameter[i].JumpSpeed = motor.JumpSpeed;
            m_pMarkParameter[i].MarkDelay = motor.MarkDelay;
            m_pMarkParameter[i].JumpDelay = motor.JumpDelay;
            m_pMarkParameter[i].PolygonDelay = motor.PolygonDelay;
            m_pMarkParameter[i].MarkCount = motor.MarkCount;

            m_pMarkParameter[i].LaserOnDelay = laser.LaserOnDelay;
            m_pMarkParameter[i].LaserOffDelay = laser.LaserOffDelay;
            m_pMarkParameter[i].DutyCycle = laser.DutyCycle;
            m_pMarkParameter[i].Frequency = laser.Frequency;
            m_pMarkParameter[i].StandbyFrequency = laser.StandbyFrequency;
            m_pMarkParameter[i].StandbyDutyCycle = laser.StandbyDutyCycle;
            m_pMarkParameter[i].LaserPower = laser.LaserPower;
            m_pMarkParameter[i].AnalogMode = laser.AnalogMode;
            m_pMarkParameter[i].PulseWidthMode = laser.PulseWidthMode;
            m_pMarkParameter[i].PulseWidth = laser.PulseWidth;
            m_pMarkParameter[i].FPKDelay = laser.FPKDelay;
            m_pMarkParameter[i].FPKLength = laser.FPKLength;
            m_pMarkParameter[i].QDelay = laser.QDelay;
            if ( laser.LaserDevice == cltSPI)
            {
                m_pMarkParameter[i].AnalogMode = 1;
                m_pMarkParameter[i].Waveform = 0;
            }
            else if ( laser.LaserDevice == cltCO2)
            {
                m_pMarkParameter[i].StandbyFrequency = 0;
                m_pMarkParameter[i].StandbyDutyCycle = 0;
            }
        }

        return m_pMarkParameter;
    }

    void CDZSTeGMCMegatron::downloadMarkFile()
    {
        checkErrorCode(HM_DownloadMarkFile(m_ipIndex, "./UDM.bin", m_hWnd), cnStr("下载UDM.BIN打标文件到控制卡的临时存储区中"));
        checkErrorCode(HM_BurnMarkFile(m_ipIndex, true), cnStr("固化脱机打标文件"));
    }

    void CDZSTeGMCMegatron::printError(const QString &p_msg)
    {
        m_error = p_msg;
        emit sigError(m_error);
    }

    bool CDZSTeGMCMegatron::checkErrorCode(int errorCode, QString preMsg)
    {
        bool ok = (errorCode == HM_OK);
        if (!ok)
        {
            printError(formatErrorMsg(preMsg, errorCode));
        }
        return ok;
    }

    void CDZSTeGMCMegatron::slotConnectDevice()
    {
        if (m_bConnected)
        {
            return;
        };
        QByteArray ipBytes = m_ip.ip.toLocal8Bit();
        if(HM_ConnectByIpStr(ipBytes.data()) == HM_OK)
        {
            m_ipIndex = HM_GetIndexByIpAddr(ipBytes.data());
            emit sigConnected();
            return;
        }
        emit sigDisconnected();
    }

    void CDZSTeGMCMegatron::slotConnected()
    {
        m_bConnected = true;
        myInfo << cnStr("%1(%2:%3:%4)已连接").arg(m_ip.name).arg(m_ip.ip).arg(m_ip.ipPort);
        m_reConnectTimes = 0;
    }

    void CDZSTeGMCMegatron::slotDisconnected()
    {
        m_bConnected = false;
        if (m_reConnectTimes % 10 == 0)
        {
            myInfo << cnStr("%1(%2:%3)已断开连接").arg(m_ip.name).arg(m_ip.ip).arg(m_ip.ipPort) + cnStr(".尝试第%1次重连").arg((m_reConnectTimes == 0) ? 1 : m_reConnectTimes);
        }
        m_reConnectTimes++;
        QTimer::singleShot(cReconnectInterval, this, [this]()
        {
            slotConnectDevice();
        });
    }

    CDZSTeGMCState* GMCState()
    {
        static CDZSTeGMCState p_GMCState;
        return &p_GMCState;
    }
}