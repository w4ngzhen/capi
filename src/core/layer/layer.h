#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/painter.h"

namespace capi {

/**
 * 事件回调EventCb
 * 退出当前层状态
 */
typedef std::function<void()> LayerEventOnQuitCurrentLayerCb;

class Layer {
public:
  explicit Layer(const Size &canvasSize);
  virtual void onMousePress(const Point &);
  virtual void onMouseMove(const Point &);
  virtual void onMouseRelease(const Point &);
  virtual void onMouseDoubleClick(const Point &);
  virtual void onKeyPress(Key, KeyboardModifier);
  virtual void onPaint(Painter *) = 0;

  virtual void onCanvasResize(const Size &);
  virtual void setLayerEventOnQuitCurrentLayerCb(LayerEventOnQuitCurrentLayerCb cb);

protected:
  Point mouse_press_pos_;
  Point mouse_current_pos_;
  Size canvas_size_;
  LayerEventOnQuitCurrentLayerCb layer_event_on_quit_current_layer_cb_;
};
} // namespace capi
