#include "capturing_layer.h"
//#include "screenshot_utils.h"
#include "helper/paint_helper.h"
#include "helper/math_helper.h"

CapturingLayer::CapturingLayer(QSize screenSize)
    : screen_size_(screenSize)
{

}

void CapturingLayer::paint(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(QColor(0, 111, 222), 2));
    int mX = this->mouse_pos_.x();
    int mY = this->mouse_pos_.y();
    int dX = this->mouse_down_pos_.x();
    int dY = this->mouse_down_pos_.y();
    QRect rect = math_helper::calcRect(mX, mY, dX, dY);
    painter.drawRect(rect);

    paint_helper::paintCapturingRectSizeTip(
                painter,
                this->mouse_pos_,
                this->mouse_down_pos_,
                rect.size());
    painter.restore();
}

void CapturingLayer::setScreenSize(const QSize &size)
{
    this->screen_size_ = QSize(size);
}

void CapturingLayer::mouseMoveEvent(QMouseEvent *event)
{
    this->mouse_pos_ = event->pos();
}

void CapturingLayer::mousePressEvent(QMouseEvent *event)
{
    this->mouse_down_pos_ = event->pos();
}

void CapturingLayer::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint &downPos = this->mouse_down_pos_;
    QPoint currentPos = event->pos();
    QRect capturedRect = math_helper::calcRect(
                currentPos.x(), currentPos.y(), downPos.x(), downPos.y());

    if (!math_helper::sizeLengthOver(capturedRect.size()))
    {
        // 截图区域过于小，不予捕获，回到Explore状态
        emit this->capturingFinishedSignal(false, nullptr);
    }
    else
    {
        emit this->capturingFinishedSignal(true, &capturedRect);
    }
}

CapturingLayer::~CapturingLayer(){}
