#include "captured_layer.h"

#include <QPainter>

CapturedLayer::CapturedLayer(QSize screenSize)
    : screen_size_(screenSize)
{

}

void CapturedLayer::setCapturedRect(QRect &rect)
{
    this->captured_rect_ = QRect(rect);
}

void CapturedLayer::paint(QPainter &painter)
{
    painter.save();

    // 将截图矩形的周围都添加灰色蒙版
    int rX = this->captured_rect_.x();
    int rY = this->captured_rect_.y();
    int rW = this->captured_rect_.width();
    int rH = this->captured_rect_.height();
    // 得到捕获区域左侧的矩形

    int screenW = this->screen_size_.width();
    int screenH = this->screen_size_.height();

    QRect leftMaskRect(0, 0, rX, screenH);
    QRect rightMaskRect(rX + rW, 0, screenW - (rX + rW), screenH);
    QRect topMaskRect(rX, 0, rW, rY);
    QRect bottomMaskRect(rX, (rY + rH), rW, screenH - (rY + rH));

    QBrush grayBrush = QBrush(QColor(0, 0, 0, 50)); // 50% Alpha的灰色
    painter.fillRect(leftMaskRect, grayBrush);
    painter.fillRect(rightMaskRect, grayBrush);
    painter.fillRect(topMaskRect, grayBrush);
    painter.fillRect(bottomMaskRect, grayBrush);

    // 增加一个边框
    painter.setPen(QPen(QColor(0, 111, 222), 2));
    painter.drawRect(this->captured_rect_);

    painter.restore();
}
