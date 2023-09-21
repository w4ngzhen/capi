#include "rect_shape.h"
#include "shape.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"

namespace capi {

/**
 * 矩形类型为0xFF01
 * @return
 */
int RectShape::type() {
  return 0xFF01;
}

RectShape::RectShape(const ShapeConfig &config) : Shape(config) {

}

void RectShape::onContentPaint(Painter *painter) {
  painter->save();
  const auto &contentRect = this->content_rect();
  auto &configRef = this->config_;
  if (config_.is_filled()) {
    painter->fillRect(contentRect, Brush(configRef.brush_color()));
  }
  painter->setPen(Pen(configRef.pen_color(), configRef.pen_width()));
  painter->drawRect(contentRect);
  painter->restore();
}

}


