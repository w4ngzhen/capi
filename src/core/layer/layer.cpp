#include "layer.h"

#include <utility>

namespace capi {
Layer::Layer(const Size &canvas_size) : canvas_size_(canvas_size) {}
void Layer::OnMousePress(const Point &pos) { this->mouse_press_pos_ = pos; }
void Layer::OnMouseMove(const Point &pos) { this->mouse_current_pos_ = pos; }
void Layer::OnMouseRelease(const Point &) {}
void Layer::OnCanvasResize(const Size &size) { this->canvas_size_ = size; }
void Layer::OnMouseDoubleClick(const Point &) {}
void Layer::OnKeyPress(Key, KeyboardModifier) {}
void Layer::SetLayerEventOnQuitCurrentLayerCb(LayerEventOnQuitCurrentLayerCb cb) {
  this->layer_event_on_quit_current_layer_cb_ = std::move(cb);
}
} // namespace capi
