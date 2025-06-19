#include "mainWindow.h"
#include "mainWindowDef.h"
#include "basic.h"
#include "system/basic.h"
#include "hal/vm/vm.h"
#include "menuBar/menuBar.h"
#include "logTextBrowser/textBrowser.h"
#include "manuPanel/manuPanel.h"
#include "imageListPanel/imageListPanel.h"
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace TIGER_VMSLM;
using namespace TIGER_UI_SLM;

CMainWindow* g_pMainWindow = nullptr;
CMainWindow* mainWindow()
{
    return g_pMainWindow;
};

CMainWindow::CMainWindow(QWidget *parent) : QWidget(parent), m_pVM(nullptr)
{
    g_pMainWindow = this;

    m_hWnd = (HWND)this->winId();
    auto pVM = new CVM(m_hWnd);
    pVM->init();
    m_pVM = pVM;

    init();
}

CMainWindow::~CMainWindow()
{
    delPtr(m_pVM);
}

void CMainWindow::printMsg(QString p_msg)
{
    const QString cFont = "<font color=\"#0000FF\">%1</font>";
    m_pOutMsg->append(cFont.arg(p_msg.replace("\n", "<br>")));
    m_pOutMsg->moveCursor(m_pOutMsg->textCursor().End);
}

bool CMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	Q_UNUSED(eventType);
    if(m_pVM)
    {
        MSG* msg = static_cast<MSG*>(message);
        m_pVM->nativeEvent(msg);
        return QWidget::nativeEvent(eventType, message, result);
    }
	return QWidget::nativeEvent(eventType, message, result);
}

void CMainWindow::init()
{
    m_pOutMsg = new CTextBrowser(this, this);
    m_pOutMsg->setStyleSheet(cStyleSheet);
    m_pOutMsg->document()->setMaximumBlockCount(300);
    m_pOutMsg->setFixedHeight(100);

    m_pManuPanel = new CManuPanel(m_pVM);
    m_pShowSlicePanel = new CImageListPanel(this, m_pVM);
    m_pMenuBar = new CMainWindowMenuBar(this);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(m_pMenuBar);
    pLayout->addWidget(m_pShowSlicePanel);
    pLayout->addWidget(m_pManuPanel);
    pLayout->addWidget(m_pOutMsg);
    this->setLayout(pLayout);
}
