#pragma once
#include "core/base/point.h"
#include "layer.h"
#include <iostream>

namespace capi {

class Painter;
class Rect;

class CapturingLayer: public Layer {

public:
  explicit CapturingLayer(const Size &);

  void onPaint(Painter *) override;

  void finishCapture(const Point &endPos);

private:
  // 静态辅助方法，便于绘制捕获过程中捕获的矩形框信息
  static void drawCaptureRectInfo(Painter *, Point &startPos, Point &currentPos);
};
} // namespace capi
