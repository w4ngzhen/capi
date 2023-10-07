#include "canvas.h"

#include <utility>

#include "./paint/painter.h"
#include "canvas_status.h"
#include "layer/captured_layer.h"
#include "layer/capturing_layer.h"
#include "layer/explore_layer.h"
#include "core/event/captured_image_save_event.h"

namespace capi {

Canvas::Canvas(Image *canvasImg) : canvas_img_(canvasImg), status_(Explore) {
  Size init_canvas_size = Size(100, 100);
  this->explore_layer_ = new ExploreLayer(this->canvas_img_, init_canvas_size);
  this->capturing_layer_ = new CapturingLayer(init_canvas_size);
  this->captured_layer_ = new CapturedLayer(init_canvas_size);

  // 注册各种事件
  // 回调事件：layer各层退出事件
  this->explore_layer_->SetLayerEventOnQuitCurrentLayerCb(
      [this] { HandleLayerEventOnLayerQuitCurrentLayer(CanvasStatus::Explore); });
  this->capturing_layer_->SetLayerEventOnQuitCurrentLayerCb(
      [this] { HandleLayerEventOnLayerQuitCurrentLayer(CanvasStatus::Capturing); });
  this->captured_layer_->SetLayerEventOnQuitCurrentLayerCb(
      [this] { HandleLayerEventOnLayerQuitCurrentLayer(CanvasStatus::Captured); });

  // 回调事件：capturing layer完成区域捕获
  this->capturing_layer_->SetLayerEventOnCapturingLayerFinishCb(
      [this](auto validSize, auto rect) { HandleLayerEventOnCapturingLayerFinish(validSize, rect); });

  this->captured_layer_->SetLayerEventOnCapturedLayerImageSaveCb([this](auto event) {
    HandleLayerEventOnCapturedLayerImageSave(event);
  });
}

void Canvas::OnPaint(Painter *painter) {
  // 首先绘制图像
  painter->DrawImage(Rect(Point(0, 0), this->size_), this->canvas_img_);

  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->OnPaint(painter);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->OnPaint(painter);
      break;
    case CanvasStatus::Captured:this->captured_layer_->OnPaint(painter);
      break;
    default:break;
  }
}

void Canvas::OnMousePress(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:
      // explore 按下鼠标，则进入capturing模式
      this->status_ = CanvasStatus::Capturing;
      this->capturing_layer_->OnMousePress(pos);
      break;
    case CanvasStatus::Capturing:break;
    case CanvasStatus::Captured:this->captured_layer_->OnMousePress(pos);
      break;
    default:break;
  }
}

void Canvas::OnMouseMove(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->OnMouseMove(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->OnMouseMove(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->OnMouseMove(pos);
      break;
    default:break;
  }
}

void Canvas::OnMouseRelease(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->OnMouseRelease(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->OnMouseRelease(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->OnMouseRelease(pos);
      break;
    default:break;
  }
}

void Canvas::OnKeyPress(Key key, KeyboardModifier m) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->OnKeyPress(key, m);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->OnKeyPress(key, m);
      break;
    case CanvasStatus::Captured:this->captured_layer_->OnKeyPress(key, m);
      break;
    default:break;
  }
}

void Canvas::OnResize(const Size &size) {
  this->size_ = size;
  this->explore_layer_->OnCanvasResize(size);
  this->capturing_layer_->OnCanvasResize(size);
  this->captured_layer_->OnCanvasResize(size);
}

void Canvas::OnMouseDoubleClick(const Point &pos) {
  switch (this->status_) {
    case CanvasStatus::Explore:this->explore_layer_->OnMouseDoubleClick(pos);
      break;
    case CanvasStatus::Capturing:this->capturing_layer_->OnMouseDoubleClick(pos);
      break;
    case CanvasStatus::Captured:this->captured_layer_->OnMouseDoubleClick(pos);
      break;
    default:break;
  }
}

/**
 * capturingLayer处理完成后，触发该处进行截取的处理
 * @param sizeValid
 * @param capturedRect
 */
void Canvas::HandleLayerEventOnCapturingLayerFinish(bool sizeValid,
                                                    const Rect &capturedRect) {
  if (!sizeValid) {
    this->status_ = CanvasStatus::Explore;
    this->captured_layer_->SetCapturedRect(Rect());
  } else {
    this->status_ = CanvasStatus::Captured;
    this->captured_layer_->SetCapturedRect(capturedRect);
  }
}

/**
 * 处理通用所有layer的quit current事件，一般会进行切换处理
 * @param status
 */
void Canvas::HandleLayerEventOnLayerQuitCurrentLayer(CanvasStatus status) {
  switch (status) {
    case CanvasStatus::Capturing:
    case CanvasStatus::Captured:
      // Capturing huo Captured，退到 Explore阶段
      this->status_ = CanvasStatus::Explore;
      return;
    case CanvasStatus::Explore:
      // explore阶段，则退出截图画布
      this->canvas_event_on_quit_cb_();
      return;
    default:break;
  }
}
/**
 * 处理captured layer 图片保存事件，这里作为canvas canvas_image save事件转发到外部
 * @param event
 */
void Canvas::HandleLayerEventOnCapturedLayerImageSave(const CapturedImageSaveEvent *event) {
  this->canvas_event_on_image_save_cb_(event);
}

void Canvas::SetCanvasEventOnQuitCb(CanvasEventOnQuitCb cb) {
  this->canvas_event_on_quit_cb_ = std::move(cb);
}

void Canvas::SetCanvasEventOnImageSaveCb(CanvasEventOnImageSaveCb cb) {
  this->canvas_event_on_image_save_cb_ = std::move(cb);
}

Canvas::~Canvas() {
  delete this->explore_layer_;
  delete this->capturing_layer_;
  delete this->captured_layer_;
}
Image *Canvas::canvas_image() {
  return this->canvas_img_;
}
Size Canvas::size() {
  return this->size_;
}
} // namespace capi
