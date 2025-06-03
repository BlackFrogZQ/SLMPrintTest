#include "controlWindow.h"
#include "system/basic.h"
#include "hal/DZSTMark/DZSTMark.h"
#include "hal/vm/ncDef.h"
#include "hal/vm/vm.h"
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace TIGER_VMSLM;

CControl::CControl(QWidget *parent, CDZSTMark *p_CDZSTMark, CVM *p_pVM): QWidget(parent), m_pCDZSTMark(p_CDZSTMark), m_pVM(p_pVM)
{
    init();
}

CControl::~CControl()
{
}

void CControl::init()
{
    m_pConnect = new QPushButton(cnStr("连接"));
    m_pDisconnect = new QPushButton(cnStr("断开"));
    m_pCreateUDM = new QPushButton(cnStr("生成UDM"));
    m_pDownloadUDM = new QPushButton(cnStr("下载UDM"));
    m_pStartMark = new QPushButton(cnStr("打标"));
    m_pPauseMark = new QPushButton(cnStr("暂停"));
    m_pContinueMark = new QPushButton(cnStr("继续"));
    m_pStopMark = new QPushButton(cnStr("停止"));
    m_pStartWork = new QPushButton(cnStr("开始工作"));
    m_pStopWork = new QPushButton(cnStr("停止工作"));
    QGridLayout *pConnectLayout = new QGridLayout;
    pConnectLayout->addWidget(m_pConnect, 0, 0, 1, 1);
    pConnectLayout->addWidget(m_pDisconnect, 0, 1, 1, 1);
    pConnectLayout->addWidget(m_pCreateUDM, 0, 2, 1, 1);
    pConnectLayout->addWidget(m_pDownloadUDM, 0, 3, 1, 1);
    pConnectLayout->addWidget(m_pStartMark, 1, 0, 1, 1);
    pConnectLayout->addWidget(m_pPauseMark, 1, 1, 1, 1);
    pConnectLayout->addWidget(m_pContinueMark, 1, 2, 1, 1);
    pConnectLayout->addWidget(m_pStopMark, 1, 3, 1, 1);
    pConnectLayout->addWidget(m_pStartWork, 2, 0, 1, 2);
    pConnectLayout->addWidget(m_pStopWork, 2, 2, 1, 2);
    connect(m_pConnect, &QPushButton::clicked, this, &CControl::OnBnClickedButtonConnect);
    connect(m_pDisconnect, &QPushButton::clicked, this, &CControl::OnBnClickedButtonDisconnect);
    connect(m_pCreateUDM, &QPushButton::clicked, this, &CControl::OnBnClickedButtonCreateUDM);
    connect(m_pDownloadUDM, &QPushButton::clicked, this, &CControl::OnBnClickedButtonDownloadUDM);
    connect(m_pStartMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStartMark);
    connect(m_pPauseMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonPauseMark);
    connect(m_pContinueMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonContinueMark);
    connect(m_pStopMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStopMark);
    connect(m_pStartWork, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStartWork);
    connect(m_pStopWork, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStopwork);

    this->setLayout(pConnectLayout);
}

void CControl::OnBnClickedButtonConnect()
{
    m_pCDZSTMark->ipConnectDevice(DZSTMarkConnectPara()->ip);
}

void CControl::OnBnClickedButtonDisconnect()
{
    m_pCDZSTMark->disconnectDevice();
}



void CControl::OnBnClickedButtonCreateUDM()
{
    m_pCDZSTMark->creatUdmBin();
}

void CControl::OnBnClickedButtonDownloadUDM()
{
    m_pCDZSTMark->downloadMarkFile();
}



void CControl::OnBnClickedButtonStartMark()
{
    m_pCDZSTMark->starMark();
}

void CControl::OnBnClickedButtonPauseMark()
{
    m_pCDZSTMark->PauseMark();
}

void CControl::OnBnClickedButtonContinueMark()
{
    m_pCDZSTMark->ContinueMark();
}

void CControl::OnBnClickedButtonStopMark()
{
    m_pCDZSTMark->StopMark();
}


void CControl::OnBnClickedButtonStartWork()
{
    m_pVM->autoWork();
}

void CControl::OnBnClickedButtonStopwork()
{
    m_pVM->stopWork();
}