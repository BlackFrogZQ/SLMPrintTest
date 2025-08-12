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
    class IMarkGraph;
    class CMarkGraphCreator;
}
namespace TIGER_UI_CONTROL
{
    class CLedLab;
}
class QLabel;
class QLineEdit;
class QComboBox;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QCheckBox;
class QButtonGroup;
class CalibrateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CalibrateDialog(QWidget *parent = nullptr, TIGER_VMSLM::CVM *p_pVM = nullptr);
    ~CalibrateDialog() override;

protected:
    void initLayout();
    void initParas();
    void initOperate();
    QVBoxLayout* initOperateLayout();
    QVBoxLayout* initParasLayout();

    void radioToggled(int p_id, bool p_checked);
    void markDatasUpdated();
    void markParasUpdated();
    void markProgressUpdated(int p_value);
    void vmStatusUpdated();

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
    TIGER_MarkGraph::IMarkGraph *m_pMarkGraph;

    QVector<QLabel*> m_pMotorParasLabels;
    QVector<QLineEdit*> m_pMotorParasEdits;
    QVector<QLabel*> m_pLaserParasLabels;
    QVector<QLineEdit*> m_pLaserParasEdits;
    QComboBox *m_pLaserDevice;

    QRadioButton *m_pRoundRadio;
    QRadioButton *m_pGridRadio;
    QRadioButton *m_pSpiralFillRadio;
    QButtonGroup* m_pShapeButtonGroup;
    QCheckBox *m_pPathIndicateCheck;
    TIGER_MarkGraph::CMarkShapeParas *m_pMarkShapeParas;
    QLineEdit *m_pMarkRangeEdit;
    QLineEdit *m_pRowAndColEdit;
    QLineEdit *m_pDiameterEdit;
    TIGER_UI_CONTROL::CLedLab *m_ledLab;

    QProgressBar *m_progress;
};