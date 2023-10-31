#include "capi_core/layer/shape/ellipse_shape.h"
#include "capi_core/layer/shape/shape.h"
#include "capi_core/paint/painter.h"
#include "capi_core/paint/pen.h"

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

void EllipseShape::OnContentPaint(Painter *painter) {
  painter->Save();
  const auto &content_rect = this->content_rect();
  auto &config = this->config_;
  if (config_.is_filled()) {
    painter->SetBrush(Brush(config.brush_color()));
    painter->DrawEllipse(content_rect);
  }
  painter->SetPen(Pen(config.pen_color(), config.pen_width()));
  painter->DrawEllipse(content_rect);
  painter->Restore();
}

}



