#include "DZSTMark.h"
#include "DZSTMarkDef.h"
#include "system/basic.h"

using namespace TIGER_DZSTMarkDef;
typedef unsigned char byte2;
#define PI 3.1415926;

CDZSTMark::CDZSTMark(HWND p_hWnd): m_hWnd(p_hWnd)
{
    m_ipIndex = 0;
    init();
}

CDZSTMark::~CDZSTMark()
{

}

void CDZSTMark::init()
{
    if(HM_InitBoard(m_hWnd) == HM_OK)
    {
        myDebug << cnStr("初始化成功");
    }
    else if(HM_InitBoard(m_hWnd) == HM_FAILED)
    {
        myDebug << cnStr("初始化失败");
    }
}

void CDZSTMark::nativeEvent(MSG* p_message)
{
	switch (p_message->message)
	{
        case HM_MSG_DeviceStatusUpdate:
        {
            deviceStatusUpdate(p_message->wParam, p_message->lParam);
            break;
        }
        case HM_MSG_StreamEnd:
        {
            UDMDownloadEnd(p_message->wParam, p_message->lParam);
            break;
        }
        case HM_MSG_UDMHalt:
        {
            break;
        }
        case HM_MSG_ExecProcess:
        {
            break;
        }
        default:
            break;
	}
}

LRESULT CDZSTMark::deviceStatusUpdate(WPARAM wParam, LPARAM lParam)
{
	QString strIP;
	byte2* ipArr = (byte2*)&wParam;
	strIP = QString("%1.%2.%3.%4").arg(ipArr[0]).arg(ipArr[1]).arg(ipArr[2]).arg(ipArr[3]);
	if (HM_DEV_Connect == HM_GetConnectStatus(lParam))
	{
		//设备已经成功连接
		myDebug << strIP + cnStr("连接成功!");
		m_ipIndex = lParam;
	}
	else if (HM_DEV_Ready == HM_GetConnectStatus(lParam))
	{
		//设备处于待机状态，即已经找到设备IP，可以进行连接
		myDebug << strIP + cnStr("处于待机状态!");
	}
	else if (HM_DEV_NotAvailable == HM_GetConnectStatus(lParam))
	{
		//未找到设备，控制卡断电或者网线未连接
		myDebug << strIP + cnStr("未找到设备!");
	}
	return 0;
}

LRESULT CDZSTMark::UDMDownloadEnd(WPARAM wParam, LPARAM lParam)
{
    myDebug << cnStr("UDM下载完成!");
	return 0;
}



// 连接指定IP索引的控制卡
void CDZSTMark::indexConnectDevice()
{
	if(HM_ConnectTo(m_ipIndex) == HM_OK)
    {
        myDebug << cnStr("连接成功");
    }
    else if(HM_ConnectTo(m_ipIndex) == HM_FAILED)
    {
        myDebug << cnStr("连接失败");
    }
}

// 通过IP地址的方式连接控制卡
void CDZSTMark::ipConnectDevice(QString p_ip)
{
    char* pIp = p_ip.toLatin1().data();
	if(HM_ConnectByIpStr(pIp) == HM_OK)
    {
        myDebug << cnStr("连接成功");
    } 
    else if(HM_ConnectByIpStr(pIp) == HM_FAILED)
    {
        myDebug << cnStr("连接失败");
    }
}

void CDZSTMark::disconnectDevice()
{
	if(HM_DisconnectTo(m_ipIndex) == HM_OK)
    {
        myDebug << cnStr("断开连接成功");
    }
    else if(HM_DisconnectTo(m_ipIndex) == HM_FAILED)
    {
        myDebug << cnStr("断开连接失败");
    }
}



void CDZSTMark::starMark()
{
	if (HM_GetConnectStatus(m_ipIndex) == HM_DEV_Connect)
	{
        HM_StartMark(m_ipIndex);
    }
    else
    {
        myInfo << cnStr("设备已断开");
    }
}

void CDZSTMark::PauseMark()
{
	if (HM_GetConnectStatus(m_ipIndex) == HM_DEV_Connect)
	{
        HM_PauseMark(m_ipIndex);
    }
    else
    {
        myInfo << cnStr("设备已断开");
    }
}

void CDZSTMark::ContinueMark()
{
	if (HM_GetConnectStatus(m_ipIndex) == HM_DEV_Connect)
	{
        HM_ContinueMark(m_ipIndex);
    }
    else
    {
        myInfo << cnStr("设备已断开");
    }
}

void CDZSTMark::StopMark()
{
	if (HM_GetConnectStatus(m_ipIndex) == HM_DEV_Connect)
	{
        HM_StopMark(m_ipIndex);
    }
    else
    {
        myInfo << cnStr("设备已断开");
    }
}



MarkParameter* CDZSTMark::getMarkParameter()
{
	MarkParameter* para = new MarkParameter[2];
    return para;
}

void CDZSTMark::creatUdmBin()
{
	UDM_NewFile();
	UDM_Main();
	// UDM_SetProtocol(ui.comboBox_protocol->currentIndex(), 0);
	UDM_SetLayersPara(getMarkParameter(), 2);
	int startAddress = UDM_RepeatStart(laserParas()->LaserDevice);

    int pRowColCount = markShapeParas()->rowAndCol;
    int pMarkRang = markShapeParas()->markRange;
    int pSpace = pRowColCount == 1 ? 0 : pMarkRang / (pRowColCount - 1);
    int x = pRowColCount == 1 ? 0 : -(pMarkRang / 2);
    int y = pRowColCount == 1 ? 0 : pMarkRang / 2;
    for (size_t row = 0; row < pRowColCount; row++)
    {
        for (size_t col = 0; col < pRowColCount; col++)
        {
            switch (markShapeParas()->shapeType)
            {
            case cmsRound:
            {
                structUdmPos polyline2d[61];
                float radius = (markShapeParas()->diameter)/2;
                // double dAngle = 2 * PI / 60;
                double dAngle = 2;
                for (int i = 0; i < 61; i++)
                {
                    polyline2d[i].x = radius * cos(dAngle * i);
                    polyline2d[i].y = radius * sin(dAngle * i);
                    polyline2d[i].z = 0;
                    polyline2d[i].a = 0;
                }
                UDM_AddPolyline2D(polyline2d, 61, 0);
                break;
            }
            case cmsCross:
            {
                structUdmPos polyline2dLine1[2] = {-60,0,0,0, 60,0,0,0 };
                UDM_AddPolyline2D(polyline2dLine1, 2, 0);
                structUdmPos polyline2dLine2[2] = {-60,0,0,0, 60,0,0,0 };
                UDM_AddPolyline2D(polyline2dLine2, 2, 0);
                break;
            }
            case cmsRectangular:
            {
                float radius = (markShapeParas()->diameter)/2;
                structUdmPos polyline2d[5] = {{radius, radius, 0, 0},
                                                {radius, -radius, 0, 0},
                                                {-radius, -radius, 0, 0},
                                                {-radius, radius, 0, 0},
                                                {radius, radius, 0, 0}};
                UDM_AddPolyline2D(polyline2d, 5, 0);
            }
            default:
                break;
            }
        }
    }

	UDM_RepeatEnd(startAddress);
	UDM_Jump(0, 0, 0);
	UDM_EndMain();

	QString strIP;
	strIP = QString("D:\\UDM%1.bin").arg(0);
	QByteArray ba = strIP.toLatin1();
	char* pPath = ba.data();
	UDM_SaveToFile(pPath);
}

void CDZSTMark::downloadMarkFile()
{
	if (HM_GetConnectStatus(m_ipIndex) == HM_DEV_Connect)
	{
        HM_DownloadMarkFile(m_ipIndex, "D:\\UDM.bin", m_hWnd);
        HM_BurnMarkFile(m_ipIndex, true);
    }
    else
    {
        myInfo << cnStr("设备已断开");
    }
}