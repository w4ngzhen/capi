#include "brush.h"

namespace capi {
Brush::Brush(const Color &color) : color_(color) {}
Brush::Brush() : color_(Color()) {}
void Brush::setColor(const Color &color) { this->color_ = color; }
Color Brush::color() const { return this->color_; }
} // namespace capi
