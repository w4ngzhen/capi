#include "core/paint/painter.h"
#include "core/base/point.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"
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
  onContentPaint(painter);
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
  painter->drawRect(this->content_rect());
  if (this->is_selected_) {
    // todo 若选中，还需绘制四个角
  }
  painter->restore();
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
const Point &Shape::start_pos() const {
  return startPos_;
}
void Shape::setStartPos(const Point &start_pos) {
  startPos_ = start_pos;
}
const Point &Shape::end_pos() const {
  return endPos_;
}
void Shape::setEndPos(const Point &end_pos) {
  endPos_ = end_pos;
}
Rect Shape::content_rect() const {
  auto contentRect = math_utils::calcRect(startPos_.x(), startPos_.y(), endPos_.x(), endPos_.y());
  return contentRect;
}

};