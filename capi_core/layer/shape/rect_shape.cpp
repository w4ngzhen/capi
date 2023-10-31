#include "capi_core/layer/shape/rect_shape.h"
#include "capi_core/layer/shape/shape.h"
#include "capi_core/paint/painter.h"
#include "capi_core/paint/pen.h"

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

void RectShape::OnContentPaint(Painter *painter) {
  painter->Save();
  const auto &content_rect = this->content_rect();
  auto &config = this->config_;
  if (config_.is_filled()) {
    painter->FillRect(content_rect, Brush(config.brush_color()));
  }
  painter->SetPen(Pen(config.pen_color(), config.pen_width()));
  painter->DrawRect(content_rect);
  painter->Restore();
}

}



