#include "setParaWindow.h"
#include "paraTree/paraModel.h"
#include "paraTree/paraTreeDelegate.h"
#include "para/define/paraDef.h"
#include "ui/mainWindow.h"

#include <QTreeView>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QScrollBar>
using namespace TIGER_ParaDef;
CSetParaWindow::CSetParaWindow(QWidget *parent)
    : INoExcCancelDialog(parent),
      m_pTreeModel(nullptr)
{
    // setWindowFlag(Qt::WindowStaysOnTopHint, CUiBasic::isTop);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    setWindowTitle(cnStr("设置参数"));
    setAutoFillBackground(true);
    m_treeView = new QTreeView;
    const QString cStyleSheet = "border:1px groove rgb(203,217,235);border-radius:3px;background:transparent;background-color: rgb(235,244,255);";
    m_treeView->setStyleSheet(cStyleSheet);
    m_treeView->setIndentation(0);
    m_treeView->setWordWrap(true);
    m_treeView->setHeaderHidden(true);
    m_pTreeModel = new CParaTreeModel(m_treeView);
    m_treeView->setItemDelegate(new CParaTreeDelegate);
    m_treeView->setModel(m_pTreeModel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    auto pSave = new QPushButton(cnStr("保存"));
    connect(pSave, &QPushButton::clicked, this, [this]()
            {m_bIsSave = true;this->close(); });
    auto pCancel = new QPushButton(cnStr("取消"));
    connect(pCancel, &QPushButton::clicked, this, [this]()
            {m_bIsSave = false;this->close(); });
    QHBoxLayout *pbLayout = new QHBoxLayout;
    pbLayout->addStretch();
    pbLayout->addWidget(pSave);
    pbLayout->addWidget(pCancel);
    pbLayout->setMargin(0);
    QVBoxLayout *hLayout = new QVBoxLayout;
    hLayout->addWidget(m_treeView);
    hLayout->addLayout(pbLayout);
    this->setLayout(hLayout);
    connect(m_treeView->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](const int &p_value)
            { m_treeView->viewport()->update(); });
    setMinimumSize(650, 550);
    setMaximumSize(mainWindow()->size() * 0.85);
}

CSetParaWindow ::~CSetParaWindow()
{
}

bool CSetParaWindow::setShowNode(IParaNode *p_node)
{
    assert(p_node != nullptr);
#ifdef Trace
    auto pNode = p_node->copyNode(false);
#else
    auto pNode = p_node->copyNode(true);
#endif

    if (pNode != nullptr)
    {
        m_pTreeModel->sepntRootNode(pNode);
        m_treeView->expandAll();
    }
    return pNode != nullptr;
}

bool CSetParaWindow::showRootNode()
{
    return setShowNode(paraRootNode());
}

bool CSetParaWindow::isSave()
{
    m_bIsSave = false;
    this->exec();
    return m_bIsSave;
}