#include "canvas.h"

#include "./base/point.h"
#include "./paint/painter.h"
#include "canvas_status.h"
#include "layer/captured/captured_layer.h"
#include "layer/capturing_layer.h"
#include "layer/explore_layer.h"

namespace capi {

Canvas::Canvas(Image *canvasImg) : canvas_img_(canvasImg), status_(Explore) {
  Size initCanvasSize = Size(100, 100);
  this->explore_layer_ = new ExploreLayer(this->canvas_img_, initCanvasSize);
  this->capturing_layer_ = new CapturingLayer(initCanvasSize);
  this->captured_layer_ = new CapturedLayer(initCanvasSize);
}

void Canvas::onPaint(Painter *painter) {
  // 首先绘制图像
  const Size &size = this->size_;
  const Image *img = this->canvas_img_;

  switch (this->status_) {
  case CanvasStatus::Explore:
    this->explore_layer_->onPaint(painter);
    break;
  case CanvasStatus::Capturing:
    this->capturing_layer_->onPaint(painter);
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->onPaint(painter);
    break;
  default:
    break;
  }
}

void Canvas::onMousePress(const Point &pos) {
  switch (this->status_) {
  case CanvasStatus::Explore:
    // explore 按下鼠标，则进入capturing模式
    this->status_ = CanvasStatus::Capturing;
    this->capturing_layer_->onMousePress(pos);
    break;
  case CanvasStatus::Capturing:
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->onMousePress(pos);
    break;
  default:
    break;
  }
}

void Canvas::onMouseMove(const Point &pos) {
  switch (this->status_) {
  case CanvasStatus::Explore:
    this->explore_layer_->onMouseMove(pos);
    break;
  case CanvasStatus::Capturing:
    this->capturing_layer_->onMouseMove(pos);
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->onMouseMove(pos);
    break;
  default:
    break;
  }
}
void Canvas::onMouseRelease(const Point &pos) {
  switch (this->status_) {
  case CanvasStatus::Explore:
    this->explore_layer_->onMouseRelease(pos);
    break;
  case CanvasStatus::Capturing:
    this->capturing_layer_->onMouseRelease(pos);
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->onMouseRelease(pos);
    break;
  default:
    break;
  }
}

void Canvas::onResize(const Size &size) {
  this->size_ = size;
  this->explore_layer_->onCanvasResize(size);
  this->capturing_layer_->onCanvasResize(size);
  this->captured_layer_->onCanvasResize(size);
}

Canvas::~Canvas() {
  delete this->explore_layer_;
  delete this->capturing_layer_;
  delete this->captured_layer_;
}

void Canvas::onKeyPress(const Key) {
    // todo
}
} // namespace capi
