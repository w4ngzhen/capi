#include "capi_core/paint/brush.h"

namespace capi {
Brush::Brush(const Color &color) : color_(color) {}
Brush::Brush() : Brush(Color()) {}
void Brush::SetColor(const Color &color) { this->color_ = color; }
Color Brush::color() const { return this->color_; }
} // namespace capi
