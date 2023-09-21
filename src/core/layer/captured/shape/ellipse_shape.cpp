#include "ellipse_shape.h"
#include "shape.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"

namespace capi {
/**
 * 椭圆type为0xFF02
 * @return
 */
int EllipseShape::type() {
  return 0xFF02;
}
EllipseShape::EllipseShape(const ShapeConfig &config) : Shape(config) {

}

void EllipseShape::onContentPaint(Painter *painter) {
  painter->save();
  const auto &contentRect = this->content_rect();
  auto &configRef = this->config_;
  if (config_.is_filled()) {
    painter->setBrush(Brush(configRef.brush_color()));
    painter->drawEllipse(contentRect);
  }
  painter->setPen(Pen(configRef.pen_color(), configRef.pen_width()));
  painter->drawEllipse(contentRect);
  painter->restore();
}

}



