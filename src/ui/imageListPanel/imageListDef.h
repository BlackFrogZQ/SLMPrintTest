#pragma once
#include <QPixmap>
enum CImageItemState : uint8_t
{
    cisUnPrint,
    cisPrinting,
    cisFinished,
    cisError,
    cisUnknow
};
static inline QPixmap showTransform(const QPixmap &p_pix)
{
    QTransform transform;
    transform.rotate(90);
    return p_pix.transformed(transform);
}