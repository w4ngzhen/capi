
#include "layer/explore_layer.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSize>

ExploreLayer::ExploreLayer(QImage *screenPic)
    : screen_pic_(screenPic)
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
    // 屏幕尺寸
    QSize screenSize = this->screen_pic_->size();
    // 垂直线
    painter.drawLine(mX, 0, mX, screenSize.height());
    // 水平线
    painter.drawLine(0, mY, screenSize.width(), mY);
    painter.restore();
}

void ExploreLayer::mouseMoveEvent(QMouseEvent *event)
{
    this->mouse_pos_ = event->pos();
}




