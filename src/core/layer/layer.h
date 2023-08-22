#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/painter.h"

namespace capi {
class Layer {
public:
  Layer(const Size &canvasSize);
  virtual void onMousePress(const Point &);
  virtual void onMouseMove(const Point &);
  virtual void onMouseRelease(const Point &);
  virtual void onPaint(Painter *) = 0;
  
  virtual void onCanvasResize(const Size &);

protected:
  Point mouse_press_pos_;
  Point mouse_current_pos_;
  Size canvas_size_;
};
} // namespace capi