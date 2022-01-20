
#include "layer/explore_layer.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSize>

ExploreLayer::ExploreLayer(QSize screenSize)
    : screen_size_(screenSize)
    , mouse_pos_(QPoint())
{

}

ExploreLayer::~ExploreLayer()
{

}

void ExploreLayer::paint(QPainter &painter)
{
    painter.save();
    // 绘制鼠标所在的位置垂直和水平线
    painter.setPen(QPen(Qt::red, 1));
    int mX = this->mouse_pos_.x();
    int mY = this->mouse_pos_.y();
    // 垂直线
    painter.drawLine(mX, 0, mX, this->screen_size_.height());
    // 水平线
    painter.drawLine(0, mY, this->screen_size_.width(), mY);
    painter.restore();
}

void ExploreLayer::setScreenSize(const QSize &size)
{
    this->screen_size_ = QSize(size);
}

void ExploreLayer::mouseMoveEvent(QMouseEvent *event)
{
    this->mouse_pos_ = event->pos();
}




