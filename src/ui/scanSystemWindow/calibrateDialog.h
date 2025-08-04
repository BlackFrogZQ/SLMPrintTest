#pragma once
#include <QDialog>
#include <QVBoxLayout>

namespace TIGER_VMSLM
{
    class CVM;
}
namespace TIGER_MarkGraph
{
    struct CMarkShapeParas;
}
class QLabel;
class QLineEdit;
class QComboBox;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QCheckBox;
class CalibrateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CalibrateDialog(QWidget *parent = nullptr, TIGER_VMSLM::CVM *p_pVM = nullptr);

protected:
    void initLayout();
    void initParas();
    void initOperate();
    QVBoxLayout* initOperateLayout();
    QVBoxLayout* initParasLayout();

protected:
    struct markParasHash
    {
        QString pName;
        QString pValue;
    };
    enum motorParas
    {
        mpMarkSpeed = 0,
        mpJumpSpeed,
        mpMarkDelay,
        mpJumpDelay,
        mpPolygonDelay,
        mpMarkCount,
        mpMax
    };
    enum laserParas
    {
        lpLaserPower = 0,
        lpLaserOnDelay,
        lpLaserOffDelay,
        lpQDelay,
        lpDutyCycle,
        lpMax
    };
    enum CMarkButtonID
    {
        cmbiStartMark = 0,
        cmbiPauseMark,
        cmbiContinueMark,
        cmbiStopMark,
        cmbiMax
    };

private:
    TIGER_VMSLM::CVM *m_pVM;
    QVector<QLabel*> m_pMotorParasLabels;
    QVector<QLineEdit*> m_pMotorParasEdits;
    QVector<QLabel*> m_pLaserParasLabels;
    QVector<QLineEdit*> m_pLaserParasEdits;
    QComboBox *m_pLaserDevice;

    QRadioButton *m_pRoundRadio;
    QRadioButton *m_pGridRadio;
    QRadioButton *m_pSpiralFillRadio;
    QCheckBox *m_pPathIndicateCheck;
    TIGER_MarkGraph::CMarkShapeParas *m_pMarkShapeParas;
    QLineEdit *m_pMarkRangeEdit;
    QLineEdit *m_pRowAndColEdit;
    QLineEdit *m_pDiameterEdit;
    QVector<QPushButton*> m_pMarkOperateBtns;

    QProgressBar *m_progress;
};