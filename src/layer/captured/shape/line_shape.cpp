#include "line_shape.h"

#include <QLine>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>

void LineShape::onPaint(QPainter *painter) {
  painter->setPen(this->pen_);
  painter->drawLine(start_pos_, end_pos_);
}

bool LineShape::isMouseHover(QMouseEvent *event) {
  auto mousePos = event->pos();
  // 对于线是否被选中，考虑将线的端点朝上下左右四个方向平移一段距离，
  // 得到一个区域
  // 判断点是否在区域内
  // TODO，这里暂时先判断点是否在线上，体验不好
  QPointF v1(mousePos.x() - this->end_pos_.x(),
             mousePos.y() - this->end_pos_.y());
  QPointF v2(this->end_pos_.x() - this->start_pos_.x(),
             this->end_pos_.y() - this->start_pos_.y());
  // 斜率一样
  qreal res = abs(v1.x() * v2.y() - v1.y() * v2.x());
  // 还要在线段内，使用一个矩形来构造
  QRect r(this->start_pos_, this->end_pos_);
  if (res < 1e-8 && r.contains(mousePos)) {
    return true;
  }
  return false;
}
