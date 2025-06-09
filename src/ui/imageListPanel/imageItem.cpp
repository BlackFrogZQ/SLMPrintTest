#include "imageItem.h"
#include "system/basic.h"
#include <QImageReader>
#include <QFileInfo>
#include <QLabel>
#include <QVBoxLayout>
CImageListItem::CImageListItem(QString imageName, QSize iconSize, CImageItemState state)
    : QListWidgetItem(nullptr, 0), m_imageName(imageName), m_state(cisUnPrint)
{
    QImageReader reader;
    reader.setScaledSize(iconSize);
    reader.setFileName(imageName);
    setIcon(QIcon(showTransform(QPixmap::fromImageReader(&reader))));
    setTextAlignment(Qt::AlignCenter);
    setText(getBaseName());
    setState(state);
}

CImageListItem::~CImageListItem()
{
}

void CImageListItem::setState(CImageItemState state)
{
    switch (state)
    {
    case cisUnPrint:
        this->setBackgroundColor(Qt::transparent);
        setToolTip(text() + cnStr(":未打印"));
        break;
    case cisPrinting:
        this->setBackgroundColor(Qt::cyan);
        setToolTip(text() + cnStr(":打印中"));
        break;
    case cisFinished:
        this->setBackgroundColor(Qt::green);
        setToolTip(text() + cnStr(":打印完成"));
        break;
    case cisError:
    default:
        this->setBackgroundColor(Qt::red);
        setToolTip(text() + cnStr(":打印错误"));
        break;
    }
    m_state = state;
}

CImageItemState CImageListItem::getState() const
{
    return m_state;
}

QString CImageListItem::getImageName() const
{
    return m_imageName;
}

// 获取图像文件名的“基础名”，也就是不包含路径、不包含扩展名的纯文件名。
QString CImageListItem::getBaseName() const
{
    QFileInfo fileInfo(m_imageName);
    return fileInfo.baseName();
}
