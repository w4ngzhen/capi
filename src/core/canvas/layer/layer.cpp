#include "layer.h"

namespace capi {
Layer::Layer(const Size &canvasSize) : canvas_size_(canvasSize) {}
void Layer::onMousePress(const Point &pos) { this->mouse_press_pos_ = pos; }
void Layer::onMouseMove(const Point &pos) {
  this->mouse_current_pos_ = pos;
}
void Layer::onMouseRelease(const Point &pos) {}
} // namespace capi
