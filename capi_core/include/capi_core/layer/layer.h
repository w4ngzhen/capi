#pragma once

#include "capi_core/base/point.h"
#include "capi_core/base/size.h"
#include "capi_core/paint/painter.h"
#include <functional>

namespace capi {

/**
 * 事件回调EventCb
 * 退出当前层状态
 */
typedef std::function<void()> LayerEventOnQuitCurrentLayerCb;

class Layer {
public:
  explicit Layer(const Size &canvas_size);
  virtual void OnMousePress(const Point &);
  virtual void OnMouseMove(const Point &);
  virtual void OnMouseRelease(const Point &);
  virtual void OnMouseDoubleClick(const Point &);
  virtual void OnKeyPress(Key, KeyboardModifier);
  virtual void OnPaint(Painter *) = 0;

  virtual void OnCanvasResize(const Size &);
  virtual void SetLayerEventOnQuitCurrentLayerCb(LayerEventOnQuitCurrentLayerCb cb);

protected:
  Point mouse_press_pos_;
  Point mouse_current_pos_;
  Size canvas_size_;
  LayerEventOnQuitCurrentLayerCb layer_event_on_quit_current_layer_cb_;
};
} // namespace capi
