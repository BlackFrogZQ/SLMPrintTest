#include "calibrateDialog.h"
#include "markGraph/markGraphDef.h"
#include "markGraph/iMarkGraph.h"
#include "markGraph/markGraphCreator.h"
#include "hal/vm/vm.h"
#include "hal/vm/ncDef.h"
#include "system/basic.h"
#include "control/btnSmart/btnSmart.h"
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QList>
#include <QComboBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QPushButton>
#include <QButtonGroup>

using namespace TIGER_MarkGraph;
using namespace TIGER_VMSLM;
using namespace TIGER_UI_BtnSmart;
const int labelWidth = 250;
const int editWidth = 100;
const int controlHeight = 30;
CalibrateDialog::CalibrateDialog(QWidget *parent, CVM *p_pVM) : QDialog(parent), m_pVM(p_pVM), m_pMarkGraph(nullptr)
{
    setWindowTitle("Calibrate");
    initLayout();
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

void CalibrateDialog::initLayout()
{
    QGroupBox *groupMarkParas = new QGroupBox(cnStr("打标参数"));
    groupMarkParas->setLayout(initParasLayout());
    groupMarkParas->setAlignment(Qt::AlignHCenter);
    groupMarkParas->setObjectName("markParasGroup");
    groupMarkParas->setStyleSheet("#markParasGroup::title {" "background-color:rgb(94, 163, 224);" "subcontrol-origin: margin;" "subcontrol-position: top center;" "padding: 0 10px;" "border-radius: 3px;" "}");

    QGroupBox *groupMarkOperate = new QGroupBox(cnStr("打标操作"));
    groupMarkOperate->setLayout(initOperateLayout());
    groupMarkOperate->setAlignment(Qt::AlignHCenter);
    groupMarkOperate->setObjectName("markOperateGroup");
    groupMarkOperate->setStyleSheet("#markOperateGroup::title {" "background-color:rgb(94, 163, 224);" "subcontrol-origin: margin;" "subcontrol-position: top center;" "padding: 0 10px;" "border-radius: 3px;" "}");

    m_progress = new QProgressBar();
    m_progress->setRange(0, 100);
    m_progress->setValue(0);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addStretch();
    pLayout->addWidget(groupMarkParas);
    pLayout->addSpacing(10);
    pLayout->addStretch();
    pLayout->addWidget(groupMarkOperate);
    pLayout->addStretch();
    pLayout->addWidget(m_progress);
    pLayout->addStretch();
    pLayout->setSpacing(2);
    pLayout->setMargin(10);
    pLayout->setSizeConstraint(QLayout::SetMinimumSize);
    this->setLayout(pLayout);
}

void CalibrateDialog::initOperate()
{
    const QHash<int, QString> cMarkBtnNames =
    {
        {cmbiStartMark, cnStr("开始打标")},
        {cmbiPauseMark, cnStr("暂停打标")},
        {cmbiContinueMark, cnStr("继续打标")},
        {cmbiStopMark, cnStr("停止打标")}
    };
    assert(cMarkBtnNames.size() == cmbiMax);
    m_pMarkOperateBtns.resize(cmbiMax);
    for (int i = 0; i < cmbiMax; i++)
    {
        const QSize cButtonSize(120, 60);
        m_pMarkOperateBtns[i] = new CBtnSmart("btn", cButtonSize);
        m_pMarkOperateBtns[i]->setText(cMarkBtnNames[i]);
    }
    connect(m_pMarkOperateBtns[cmbiStartMark], &CBtnSmart::sigClick, this, [this]
        {
            assert(m_pVM);
            updateMarkParas();
            updateMarkDatas();
            m_pVM->startMark();
        });

    m_pMarkShapeParas = new CMarkShapeParas;
    m_pMarkRangeEdit = new QLineEdit(QString::number(m_pMarkShapeParas->markRange));
    m_pMarkRangeEdit->setFixedSize(editWidth, controlHeight);
    m_pRowAndColEdit = new QLineEdit(QString::number(m_pMarkShapeParas->rowAndCol));
    m_pRowAndColEdit->setFixedSize(editWidth, controlHeight);
    m_pDiameterEdit = new QLineEdit(QString::number(m_pMarkShapeParas->diameter));
    m_pDiameterEdit->setFixedSize(editWidth, controlHeight);

    m_pRoundRadio = new QRadioButton(cnStr("圆形"));
    m_pGridRadio = new QRadioButton(cnStr("网格"));
    m_pSpiralFillRadio = new QRadioButton(cnStr("螺旋填充"));
    m_pPathIndicateCheck = new QCheckBox(cnStr("显示路径指示"));
    m_pShapeButtonGroup = new QButtonGroup(this);
    m_pShapeButtonGroup->addButton(m_pRoundRadio, 0);
    m_pShapeButtonGroup->addButton(m_pGridRadio, 1);
    m_pShapeButtonGroup->addButton(m_pSpiralFillRadio, 2);
    connect(m_pShapeButtonGroup, QOverload<int, bool>::of(&QButtonGroup::idToggled), this, &CalibrateDialog::radioToggled);
    m_pPathIndicateCheck->setChecked(true);
    m_pRoundRadio->setChecked(true);
}

QVBoxLayout* CalibrateDialog::initOperateLayout()
{
    initOperate();

    QHBoxLayout *pMarkShapeLayout = new QHBoxLayout;
    pMarkShapeLayout->addWidget(new QLabel(cnStr("打标图形：")));
    pMarkShapeLayout->addWidget(m_pRoundRadio);
    pMarkShapeLayout->addWidget(m_pGridRadio);
    pMarkShapeLayout->addWidget(m_pSpiralFillRadio);
    pMarkShapeLayout->addWidget(m_pPathIndicateCheck);

    QHBoxLayout *pShapeParasLayout = new QHBoxLayout;
    pShapeParasLayout->addWidget(new QLabel(cnStr("形状参数：")));
    pShapeParasLayout->addStretch();
    pShapeParasLayout->addWidget(new QLabel(cnStr("打标范围(mm)")));
    pShapeParasLayout->addWidget(m_pMarkRangeEdit);
    pShapeParasLayout->addStretch();
    pShapeParasLayout->addWidget(new QLabel(cnStr("行列数")));
    pShapeParasLayout->addWidget(m_pRowAndColEdit);
    pShapeParasLayout->addStretch();
    pShapeParasLayout->addWidget(new QLabel(cnStr("直径(mm)")));
    pShapeParasLayout->addWidget(m_pDiameterEdit);

    QHBoxLayout *pBtnsLayout = new QHBoxLayout;
    pBtnsLayout->addStretch();
    pBtnsLayout->addWidget(m_pMarkOperateBtns[cmbiStartMark]);
    pBtnsLayout->addStretch();
    pBtnsLayout->addWidget(m_pMarkOperateBtns[cmbiPauseMark]);
    pBtnsLayout->addStretch();
    pBtnsLayout->addWidget(m_pMarkOperateBtns[cmbiContinueMark]);
    pBtnsLayout->addStretch();
    pBtnsLayout->addWidget(m_pMarkOperateBtns[cmbiStopMark]);
    pBtnsLayout->addStretch();

    QVBoxLayout *pOperateLayout = new QVBoxLayout;
    pOperateLayout->addStretch();
    pOperateLayout->addLayout(pMarkShapeLayout);
    pOperateLayout->addStretch();
    pOperateLayout->addLayout(pShapeParasLayout);
    pOperateLayout->addStretch();
    pOperateLayout->addLayout(pBtnsLayout);
    pOperateLayout->addStretch();
    pOperateLayout->setSpacing(15);
    pOperateLayout->setMargin(10);
    pOperateLayout->setSizeConstraint(QLayout::SetMinimumSize);
    return pOperateLayout;
}

void CalibrateDialog::initParas()
{
    m_pMotorParasEdits.resize(mpMax);
    m_pMotorParasLabels.resize(mpMax);
    m_pLaserParasEdits.resize(lpMax);
    m_pLaserParasLabels.resize(lpMax);

    const QHash<int, markParasHash> cMotorparasNames =
    {
        {mpMarkSpeed, {cnStr("打标速度(mm/s)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkSpeed)}},
        {mpJumpSpeed, {cnStr("跳转速度(mm/s)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.JumpSpeed)}},
        {mpMarkDelay, {cnStr("打标延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkDelay)}},
        {mpJumpDelay, {cnStr("跳转延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.JumpDelay)}},
        {mpPolygonDelay, {cnStr("转弯延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.PolygonDelay)}},
        {mpMarkCount, {cnStr("打标次数"), QString::number(getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkCount)}}
    };
    const QHash<int, markParasHash> cLaserParasNames =
    {
        {lpLaserPower, {cnStr("激光能量百分比(0~100)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserPower)}},
        {lpLaserOnDelay, {cnStr("开激光延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserOnDelay)}},
        {lpLaserOffDelay, {cnStr("关激光延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserOffDelay)}},
        {lpQDelay, {cnStr("出光Q频率延时(us)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].laserParas.QDelay)}},
        {lpDutyCycle, {cnStr("出光时占空比(0~1)"), QString::number(getMarkDatas()->markParas[cmtMarkTest].laserParas.DutyCycle)}}
    };

    for(int i = 0; i < mpMax; i++)
    {
        m_pMotorParasLabels[i] = new QLabel(cMotorparasNames[i].pName);
        m_pMotorParasEdits[i] = new QLineEdit(cMotorparasNames[i].pValue);
    }
    for(int i = 0; i < lpMax; i++)
    {
        m_pLaserParasLabels[i] = new QLabel(cLaserParasNames[i].pName);
        m_pLaserParasEdits[i] = new QLineEdit(cLaserParasNames[i].pValue);
    }
    m_pLaserDevice = new QComboBox;
    m_pLaserDevice->addItems({cnStr("IPG"), cnStr("SPI"), cnStr("CO2/紫外"), cnStr("其他")});
}

QVBoxLayout* CalibrateDialog::initParasLayout()
{
    initParas();
    auto fixSize = [&](QWidget *p_widget, int p_width)
    {
        p_widget->setFixedSize(p_width, controlHeight);
    };

    auto createGridLayout = [&](const QVector<QLabel*> &labels, const QVector<QLineEdit*> &edits) -> QGridLayout*
    {
        QGridLayout *layout = new QGridLayout;
        const int columnCount = 2;

        for (int i = 0; i < labels.size(); ++i)
        {
            int row = i / columnCount;
            int col = (i % columnCount) * 2;

            fixSize(labels[i], labelWidth);
            fixSize(edits[i], editWidth);

            layout->addWidget(labels[i], row, col);
            layout->addWidget(edits[i], row, col + 1);
        }
        layout->setHorizontalSpacing(10);
        layout->setVerticalSpacing(8);
        return layout;
    };

    QGroupBox *groupMotorParas = new QGroupBox(cnStr("电机参数"));
    groupMotorParas->setObjectName("motorParasGroup");
    groupMotorParas->setAlignment(Qt::AlignLeft);
    groupMotorParas->setLayout(createGridLayout(m_pMotorParasLabels, m_pMotorParasEdits));
    QGridLayout *laserLayout = createGridLayout(m_pLaserParasLabels, m_pLaserParasEdits);
    QLabel *laserTypeLabel = new QLabel(cnStr("激光器类型"));
    fixSize(laserTypeLabel, labelWidth);
    fixSize(m_pLaserDevice, editWidth);
    laserLayout->addWidget(laserTypeLabel, 2, 2);
    laserLayout->addWidget(m_pLaserDevice, 2, 3);
    QGroupBox *groupLaserParas = new QGroupBox(cnStr("激光参数"));
    groupLaserParas->setLayout(laserLayout);
    groupLaserParas->setObjectName("laserParasGroup");

    QVBoxLayout *parasLayout = new QVBoxLayout;
    parasLayout->addWidget(groupMotorParas);
    parasLayout->addSpacing(10);
    parasLayout->addWidget(groupLaserParas);
    parasLayout->addStretch();
    parasLayout->setSpacing(10);
    parasLayout->setContentsMargins(10, 10, 10, 10);
    return parasLayout;
}

void CalibrateDialog::radioToggled(int p_id, bool p_checked)
{
    if (!p_checked) return;
    assert(m_pMarkShapeParas != nullptr);
    m_pMarkShapeParas->shapeType = static_cast<TIGER_MarkGraph::CMarkShape>(p_id);

    if (m_pMarkGraph)
    {
        delete m_pMarkGraph;
        m_pMarkGraph = nullptr;
    }
    m_pMarkGraph = TIGER_MarkGraph::CMarkGraphCreator::createMarkGraph(m_pMarkShapeParas->shapeType);
    assert(m_pMarkGraph != nullptr);
}

void CalibrateDialog::updateMarkDatas()
{
    assert(m_pMarkGraph);
    m_pMarkShapeParas->markRange = m_pMarkRangeEdit->text().toDouble();
    m_pMarkShapeParas->rowAndCol = m_pRowAndColEdit->text().toUInt();
    m_pMarkShapeParas->diameter = m_pDiameterEdit->text().toDouble();
    m_pMarkShapeParas->showPathIndicate = m_pPathIndicateCheck->isChecked();
    getMarkDatas()->printDatas = *m_pMarkGraph->getGraphDatas(m_pMarkShapeParas);
}

void CalibrateDialog::updateMarkParas()
{
    getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkSpeed = m_pMotorParasEdits[mpMarkSpeed]->text().toUInt();
    getMarkDatas()->markParas[cmtMarkTest].motorParas.JumpSpeed = m_pMotorParasEdits[mpJumpSpeed]->text().toUInt();
    getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkDelay = m_pMotorParasEdits[mpMarkDelay]->text().toUInt();
    getMarkDatas()->markParas[cmtMarkTest].motorParas.JumpDelay = m_pMotorParasEdits[mpJumpDelay]->text().toUInt();
    getMarkDatas()->markParas[cmtMarkTest].motorParas.PolygonDelay = m_pMotorParasEdits[mpPolygonDelay]->text().toUInt();
    getMarkDatas()->markParas[cmtMarkTest].motorParas.MarkCount = m_pMotorParasEdits[mpMarkCount]->text().toUInt();

    getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserPower = m_pLaserParasEdits[lpLaserPower]->text().toDouble();
    getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserOnDelay = m_pLaserParasEdits[lpLaserOnDelay]->text().toDouble();
    getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserOffDelay = m_pLaserParasEdits[lpLaserOffDelay]->text().toDouble();
    getMarkDatas()->markParas[cmtMarkTest].laserParas.QDelay = m_pLaserParasEdits[lpQDelay]->text().toDouble();
    getMarkDatas()->markParas[cmtMarkTest].laserParas.DutyCycle = m_pLaserParasEdits[lpDutyCycle]->text().toDouble();
    getMarkDatas()->markParas[cmtMarkTest].laserParas.LaserDevice = static_cast<CLaserDevice>(m_pLaserDevice->currentIndex());

    getMarkDatas()->isSLCDatas = false;
}