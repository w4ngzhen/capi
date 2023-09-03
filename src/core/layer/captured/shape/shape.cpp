#include "core/base/rect.h"
#include "core/paint/painter.h"
#include "core/base/point.h"
#include "core/paint/pen.h"
#include "shape.h"

namespace capi {

Shape::Shape(const Color &bc, const Color &pc, int pw)
    : brush_color_(bc), pen_color_(pc), pen_width_(pw) {
}

TouchedArea Shape::checkTouchedArea(const Point &mousePos) const {
  return None;
}

void Shape::onPaint(Painter *painter) {
  onBorderPaint(painter);
  onBodyPaint(painter);
}

void Shape::onBorderPaint(Painter *painter) {
  // 默认根据当前是否被hover和是否被选中来进行绘制整个图形的边框
  if (!this->is_hover_ && !this->is_selected_) {
    return;
  }
  painter->save();
  // 绘制边框
  Pen pen(1, Color(200, 200, 200), PenStyle::DashLine);
  if (this->is_selected_) {
    pen.setColor(Color(200, 200, 200));
  } else {
    pen.setColor(Color(100, 100, 100));
  }
  painter->setPen(pen);
  painter->drawRect(this->body_);
  if (this->is_selected_) {
    // todo 若选中，还需绘制四个角
  }
  painter->restore();
}

Rect &Shape::body() {
  return body_;
}
bool Shape::is_selected() const {
  return is_selected_;
}

void Shape::setIsHover(bool isHover) {
  is_hover_ = isHover;
}

void Shape::setIsSelected(bool isSelected) {
  is_selected_ = isSelected;
}
void Shape::setBrushColor(const Color &brush_color) {
  brush_color_ = brush_color;
}
void Shape::setPenColor(const Color &pen_color) {
  pen_color_ = pen_color;
}
void Shape::setPenWidth(int pen_width) {
  pen_width_ = pen_width;
}

};