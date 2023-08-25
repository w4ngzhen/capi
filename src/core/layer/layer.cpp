#include "layer.h"

#include <utility>

namespace capi {
Layer::Layer(const Size &canvasSize) : canvas_size_(canvasSize) {}
void Layer::onMousePress(const Point &pos) { this->mouse_press_pos_ = pos; }
void Layer::onMouseMove(const Point &pos) { this->mouse_current_pos_ = pos; }
void Layer::onMouseRelease(const Point &pos) {}
void Layer::onCanvasResize(const Size &size) { this->canvas_size_ = size; }
void Layer::onMouseDoubleClick(const Point &pos) {}
void Layer::onKeyPress(Key, KeyboardModifier) {}
void Layer::setLayerEventOnQuitCurrentLayerCb(LayerEventOnQuitCurrentLayerCb cb) {
  this->layer_event_on_quit_current_layer_cb_ = std::move(cb);
}
} // namespace capi
