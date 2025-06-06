#include "controlWindow.h"
#include "system/basic.h"
#include "hal/vm/ncDef.h"
#include "hal/vm/vm.h"
#include <QPushButton>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace TIGER_VMSLM;

CControl::CControl(QWidget *parent, CVM *p_pVM): QWidget(parent),  m_pVM(p_pVM)
{
    init();
}

CControl::~CControl()
{
}

void CControl::init()
{
    m_pStartMark = new QPushButton(cnStr("打标"));
    m_pPauseMark = new QPushButton(cnStr("暂停"));
    m_pContinueMark = new QPushButton(cnStr("继续"));
    m_pStopMark = new QPushButton(cnStr("停止"));
    m_pStartWork = new QPushButton(cnStr("开始工作"));
    m_pStopWork = new QPushButton(cnStr("停止工作"));
    QGridLayout *pConnectLayout = new QGridLayout;
    pConnectLayout->addWidget(m_pStartMark, 0, 0, 1, 1);
    pConnectLayout->addWidget(m_pPauseMark, 0, 1, 1, 1);
    pConnectLayout->addWidget(m_pContinueMark, 0, 2, 1, 1);
    pConnectLayout->addWidget(m_pStopMark, 0, 3, 1, 1);
    pConnectLayout->addWidget(m_pStartWork, 1, 0, 1, 2);
    pConnectLayout->addWidget(m_pStopWork, 1, 2, 1, 2);

    connect(m_pStartMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStartMark);
    connect(m_pPauseMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonPauseMark);
    connect(m_pContinueMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonContinueMark);
    connect(m_pStopMark, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStopMark);
    connect(m_pStartWork, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStartWork);
    connect(m_pStopWork, &QPushButton::clicked, this, &CControl::OnBnClickedButtonStopwork);

    this->setLayout(pConnectLayout);
}

void CControl::OnBnClickedButtonStartMark()
{
    // m_pCDZSTMark->starMark();
}

void CControl::OnBnClickedButtonPauseMark()
{
    // m_pCDZSTMark->PauseMark();
}

void CControl::OnBnClickedButtonContinueMark()
{
    // m_pCDZSTMark->ContinueMark();
}

void CControl::OnBnClickedButtonStopMark()
{
    // m_pCDZSTMark->StopMark();
}


void CControl::OnBnClickedButtonStartWork()
{
    m_pVM->autoWork();
}

void CControl::OnBnClickedButtonStopwork()
{
    m_pVM->stopWork();
}