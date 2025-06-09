#pragma once
#include <QListWidgetItem>
#include "imageListDef.h"
class QLabel;
class CImageListItem : public QListWidgetItem
{
    Q_ENUMS(CImageItemState)

public:
    CImageListItem(QString imageName, QSize iconSize = QSize(200, 200), CImageItemState state = cisUnPrint);
    ~CImageListItem();

    void setState(CImageItemState state);
    CImageItemState getState() const;
    QString getImageName() const;

protected:
    QString getBaseName() const;

protected:
    CImageItemState m_state;
    QString m_imageName;
};
