#include "canvas.h"

#include <utility>

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

  // 注册各种事件
  // 回调事件：layer各层退出事件
  this->explore_layer_->setEventCbOnQuitCurrentLayer(
      [this] { eventCbHandleOnQuitCurrentLayer(CanvasStatus::Explore); });
  this->capturing_layer_->setEventCbOnQuitCurrentLayer(
      [this] { eventCbHandleOnQuitCurrentLayer(CanvasStatus::Capturing); });
  this->captured_layer_->setEventCbOnQuitCurrentLayer(
      [this] { eventCbHandleOnQuitCurrentLayer(CanvasStatus::Captured); });

  // 回调事件：capturing layer完成区域捕获
  this->capturing_layer_->setEventCbOnCapturingFinish(
      [this](auto validSize, auto rect) { eventCbHandleOnCapturingFinish(validSize, rect); });
}

void Canvas::onPaint(Painter *painter) {
  // 首先绘制图像
  painter->drawImage(Rect(Point(0, 0), this->size_), this->canvas_img_);

  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->onPaint(painter);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->onPaint(painter);
      break;
    case CanvasStatus::Captured:this->captured_layer_->onPaint(painter);
      break;
    default:break;
  }
}

void Canvas::onMousePress(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:
      // explore 按下鼠标，则进入capturing模式
      this->status_ = CanvasStatus::Capturing;
      this->capturing_layer_->onMousePress(pos);
      break;
    case CanvasStatus::Capturing:break;
    case CanvasStatus::Captured:this->captured_layer_->onMousePress(pos);
      break;
    default:break;
  }
}

void Canvas::onMouseMove(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->onMouseMove(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->onMouseMove(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->onMouseMove(pos);
      break;
    default:break;
  }
}

void Canvas::onMouseRelease(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->onMouseRelease(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->onMouseRelease(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->onMouseRelease(pos);
      break;
    default:break;
  }
}

void Canvas::onKeyPress(Key key, KeyboardModifier m) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->onKeyPress(key, m);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->onKeyPress(key, m);
      break;
    case CanvasStatus::Captured:this->captured_layer_->onKeyPress(key, m);
      break;
    default:break;
  }
}

void Canvas::onResize(const Size &size) {
  this->size_ = size;
  this->explore_layer_->onCanvasResize(size);
  this->capturing_layer_->onCanvasResize(size);
  this->captured_layer_->onCanvasResize(size);
}

void Canvas::onMouseDoubleClick(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->onMouseDoubleClick(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->onMouseDoubleClick(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->onMouseDoubleClick(pos);
      break;
    default:break;
  }
}

/**
 * capturingLayer处理完成后，触发该处进行截取的处理
 * @param sizeValid
 * @param capturedRect
 */
void Canvas::eventCbHandleOnCapturingFinish(bool sizeValid,
                                            const Rect &capturedRect) {
  if (!sizeValid) {
    this->status_ = CanvasStatus::Explore;
    this->captured_layer_->setCapturedRect(Rect());
  } else {
    this->status_ = CanvasStatus::Captured;
    this->captured_layer_->setCapturedRect(capturedRect);
  }
}

void Canvas::eventCbHandleOnQuitCurrentLayer(CanvasStatus status) {
  switch (status) {
    case CanvasStatus::Capturing:
    case CanvasStatus::Captured:
      // Capturing huo Captured，退到 Explore阶段
      this->status_ = CanvasStatus::Explore;
      return;
    case CanvasStatus::Explore:
      // explore阶段，则退出截图画布
      this->on_canvas_quit_cb_();
      return;
    default:break;
  }
}

void Canvas::setOnCanvasQuitCb(OnCanvasQuitCb cb) {
  this->on_canvas_quit_cb_ = std::move(cb);
}

void Canvas::setOnCanvasImageSaveCb(OnCanvasImageSaveCb cb) {
  this->on_canvas_image_save_cb_ = std::move(cb);
}

Canvas::~Canvas() {
  delete this->explore_layer_;
  delete this->capturing_layer_;
  delete this->captured_layer_;
}
} // namespace capi
