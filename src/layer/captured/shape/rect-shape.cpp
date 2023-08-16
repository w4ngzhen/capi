#include "rect-shape.h"
#include <QPainter>

void RectShape::onPaint(QPainter *painter) {
  if (this->isFilled_) {
    painter->fillRect(this->body_, this->brush_);
  } else {
    painter->setPen(this->pen_);
    painter->drawRect(this->body_);
  }
}
