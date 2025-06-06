#pragma once
#include <QWidget>

class QTextBrowser;
class QPushButton;
namespace TIGER_VMSLM
{
    class CVM;
}

class CControl: public QWidget
{
    Q_OBJECT
public:
    CControl(QWidget *parent = nullptr, TIGER_VMSLM::CVM *p_pVM = nullptr);
    ~CControl();

protected:
	void init();

protected slots:
    void OnBnClickedButtonStartMark();
    void OnBnClickedButtonPauseMark();
    void OnBnClickedButtonContinueMark();
    void OnBnClickedButtonStopMark();

    void OnBnClickedButtonStartWork();
    void OnBnClickedButtonStopwork();

private:
    TIGER_VMSLM::CVM *m_pVM;

    QPushButton *m_pStartMark;
    QPushButton *m_pPauseMark;
    QPushButton *m_pContinueMark;
    QPushButton *m_pStopMark;

    QPushButton *m_pStartWork;
    QPushButton *m_pStopWork;
};
