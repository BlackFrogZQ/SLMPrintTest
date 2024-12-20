#include "basic.h"
#include "messageBox/messageBox.h"
#include <QLayout>
#include <QToolTip>
#include <QKeyEvent>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QPainterPath>

int warningBox(QWidget *parent, const QString &title, const QString &text)
{
    return CMessageBox::warning(parent, title, text);
}
void showToolTip(QWidget *p_pWidget, const QString &p_tip)
{
    QFont font = QToolTip::font();
    font.setBold(true);
    font.setPointSize(12);
    QToolTip::setFont(font);
    int w = QFontMetrics(font).maxWidth();
    const QString st = "<b style=\"color:red; bordet-radius:5px; background-color: white;\">%1</b>";
    QToolTip::showText(QCursor::pos() + QPoint(w, 0), st.arg(p_tip), p_pWidget, QRect(), 3000);
    myInfo << "showToolTip" << p_tip;
}

void deleteLayout(QLayout *p_layout)
{
    if (p_layout == nullptr)
    {
        return;
    }

    int itemCount = p_layout->count();
    for (int i = (itemCount - 1); i >= 0; --i)
    {
        QLayoutItem *item = p_layout->takeAt(i);
        if (item != NULL)
        {
            p_layout->removeWidget(item->widget());
        }
    }
    delete p_layout;
    p_layout = nullptr;
};

void IDialog::paintEvent(QPaintEvent *event)
{
    QPainter paint;
    paint.begin(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 5, 5);
    // Qt默认颜色

    paint.fillPath(path, QColor(235, 244, 255));
    paint.setPen(QPen(QColor(197, 197, 197), 2));
    paint.drawPath(path);
    QDialog::paintEvent(event);
}

void INoExcCancelDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        break;
    default:
        IDialog::keyPressEvent(event);
    }
}

double CUiBasic::stScale = CUiBasic::getDefScale();
bool CUiBasic::isTop = false;
double CUiBasic::getDefScale()
{
    constexpr int cImageW = 1626;
    constexpr int cImageH = 1236;
    constexpr double cScale = 1.0 * cImageW / cImageH;
    return cScale; // 1440/1080
}

QPixmap CUiBasic::getBackgroundImg()
{
    return QPixmap(":/res/login.jpg");
}

double CUiBasic::getScale()
{
    return stScale;
}

void CUiBasic::updateScale(int p_w, int p_h)
{
    int W, H;
    getScreen(W, H);
    QSize defSize(p_w, p_h);
    defSize.scale(W, H, Qt::KeepAspectRatio);
    if (defSize.width() == W)
    {
        stScale = 1.0 * W / p_w;
    }
    else
    {
        stScale = 1.0 * H / p_h;
    }

    // stScale = 1.0 * W / p_w;
    // if (1.0 * W / H > getDefScale())
    // {
    //     stScale = 1.0 * H / p_h;
    // }
    // stScale = (stScale < 1.0 ? 1 : stScale);
}

void CUiBasic::getScreen(int &p_w, int &p_h)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect size = screen->availableGeometry();
    p_w = size.width();
    p_h = size.height();
    // screen->primaryOrientation();
}



#include <QMouseEvent>
#include <QPainter>

CDragMainWidgetLabel::CDragMainWidgetLabel(QWidget *p_pParent)
    : QLabel(p_pParent), m_canDragMove(true), m_isDragMove(false), m_isOnlyShowImage(false)
{
    setAlignment(Qt::AlignCenter);
}

CDragMainWidgetLabel::~CDragMainWidgetLabel()
{
}

void CDragMainWidgetLabel::setCanMove(bool p_canMove)
{
    m_canDragMove = p_canMove;
}

void CDragMainWidgetLabel::slotSetOnlyShowImage(bool p_isOnlyShowImage)
{
    m_isOnlyShowImage = p_isOnlyShowImage;
    emit sigOnlyShowImage(m_isOnlyShowImage);
}

void CDragMainWidgetLabel::leaveEvent(QEvent *event)
{
    m_isDragMove = false;
}

void CDragMainWidgetLabel::mousePressEvent(QMouseEvent *ev)
{
    if (m_canDragMove && (ev->button() & Qt::LeftButton) && (!m_isDragMove))
    {
        m_isDragMove = true;
        m_lastPoint = ev->globalPos();
        return;
    }
    QLabel::mousePressEvent(ev);
}

void CDragMainWidgetLabel::mouseMoveEvent(QMouseEvent *p_event)
{
    if (m_canDragMove && m_isDragMove && (p_event->buttons() & Qt::LeftButton))
    {
        emit sigDragMove(p_event->globalPos() - m_lastPoint);
        m_lastPoint = p_event->globalPos();
        return;
    }
    QLabel::mouseMoveEvent(p_event);
}

void CDragMainWidgetLabel::mouseReleaseEvent(QMouseEvent *p_event)
{
    if (m_isDragMove && (p_event->button() & Qt::LeftButton))
    {
        m_isDragMove = false;
        return;
    }
    QLabel::mouseReleaseEvent(p_event);
}

void CDragMainWidgetLabel::mouseDoubleClickEvent(QMouseEvent *p_event)
{
    if ((p_event->buttons() & Qt::LeftButton))
    {
        slotSetOnlyShowImage(!m_isOnlyShowImage);
        emit sigMouseDoubleClick();
        return;
    }
    QLabel::mouseDoubleClickEvent(p_event);
}

void CDragMainWidgetLabel::resizeEvent(QResizeEvent *p_event)
{
    QLabel::resizeEvent(p_event);
    emit sigResize(p_event->size(),p_event->oldSize());
}