#include "line_shape.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"
namespace capi {
/**
 * 线type为0xFF03
 * @return
 */
int LineShape::type() {
  return 0xFF03;
}
LineShape::LineShape(const ShapeConfig &config) : Shape(config) {}

bool LineShape::is_line_shape() const {
  return true;
}
void LineShape::onContentPaint(Painter *painter) {
  painter->setPen(Pen(config_.pen_color(), config_.pen_width()));
  painter->drawLine(startPos_, endPos_);
}
}
