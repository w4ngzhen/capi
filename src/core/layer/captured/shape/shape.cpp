#include "core/base/rect.h"
#include "core/paint/painter.h"
#include "core/base/point.h"
#include "shape.h"

namespace capi {

Rect &Shape::body() {
  return this->body_;
}
bool Shape::is_selected() const {
  return this->is_selected_;
}

TouchedArea Shape::canBeTouched(Point &mousePos) const {
  return None;
}

void Shape::setIsHover(bool isHover) {
  this->is_hover_ = isHover;
}

void Shape::setIsSelected(bool isSelected) {
  this->is_selected_ = isSelected;
}
void Shape::onPaint(Painter *painter) {

}

};