#pragma once

#include "capi_core/paint/painter.h"
#include "capi_core/base/point.h"

namespace capi {
class ShapeConfigPane {
  void OnPaint(Painter *painter);
  void OnMousePress(Point &pos);
  void SetPosition(Point &pos);
 private:
  /**
   * 控制当前面板显示或隐藏
   */
  bool visible_;
  /**
   * 当前面板位置
   */
  Point pos_;
};
}

