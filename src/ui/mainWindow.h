#pragma once
#include "mainWindowDef.h"
#include <qt_windows.h>
#include <windows.h>
#include <windowsx.h>
#include <QWidget>

class QTextBrowser;
class CMainWindowMenuBar;
namespace TIGER_VMSLM
{
    class CVM;
}

class CMainWindow: public QWidget
{
    Q_OBJECT
public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

	void printMsg(QString p_msg);
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);

protected:
	void init();

private:
    HWND m_hWnd;
    TIGER_VMSLM::CVM *m_pVM;

	QTextBrowser* m_pOutMsg;
    QWidget* m_pControl;
    CMainWindowMenuBar *m_pMenuBar;
};

CMainWindow* mainWindow();