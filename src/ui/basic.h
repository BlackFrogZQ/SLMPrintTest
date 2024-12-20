#pragma once
#include "system/basic.h"
#include <QDialog>
class QLayout;
int warningBox(QWidget *parent, const QString &title, const QString &text);
void showToolTip(QWidget *p_pWidget, const QString &p_tip);
void deleteLayout(QLayout *p_layout);

class IDialog : public QDialog
{
public:
    using QDialog::QDialog;
protected:
    virtual void paintEvent(QPaintEvent * event) override;
};

class INoExcCancelDialog : public IDialog
{
public:
    using IDialog::IDialog;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

class CUiBasic
{
public:
    static double getDefScale();
    static QPixmap getBackgroundImg();
    static double getScale();
    static void updateScale(int p_w, int p_h);
    static bool isTop;

protected:
    static void getScreen(int &p_w, int &p_h);
    static double stScale;
};


#include <QLabel>
class QMouseEvent;

class CDragMainWidgetLabel : public QLabel
{
    Q_OBJECT
public:
    CDragMainWidgetLabel(QWidget *p_pParent = nullptr);
    virtual ~CDragMainWidgetLabel();
    void setCanMove(bool p_canMove);
    QPixmap *pixmap() = delete;

public slots:
    void slotSetOnlyShowImage(bool p_isOnlyShowImage);

signals:
    void sigDragMove(QPoint p_move);
    void sigOnlyShowImage(bool p_isOnlyShowImage);
    void sigMouseDoubleClick();
    void sigResize(QSize p_newSize,QSize p_oldSize);

protected:
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseDoubleClickEvent(QMouseEvent *ev) override;
    void resizeEvent(QResizeEvent *event) override;

protected:
    bool m_canDragMove;
    bool m_isDragMove;
    bool m_isOnlyShowImage;
    QPoint m_lastPoint;
};