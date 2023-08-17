#include "rect_shape.h"
#include <QMouseEvent>
#include <QPainter>

void RectShape::onPaint(QPainter *painter) {
  if (this->isFilled_) {
    painter->fillRect(this->rect_, this->brush_);
  } else {
    painter->setPen(this->pen_);
    painter->drawRect(this->rect_);
  }
}

bool RectShape::isMouseHover(QMouseEvent *event) {
  auto pos = event->pos();
  // 无论矩形的绘制样式是填充还是一般边框，
  // 都只考虑点是否落到了矩形框整个区域即可。
  return this->rect_.contains(pos);
}
