#pragma once
#include "core/base/point.h"
#include "layer.h"
#include <iostream>

namespace capi {

class Painter;
class Rect;

/**
 * 事件回调EventCb
 * 区域捕获完成事件
 */
typedef std::function<void(bool sizeValid, const Rect &capturedRect)>
    EventCbOnCapturingFinish;

class CapturingLayer : public Layer {

public:
  explicit CapturingLayer(const Size &);

  void onPaint(Painter *) override;
  void onMouseRelease(const Point &) override;
  void onKeyPress(Key, KeyboardModifier) override;

  void setEventCbOnCapturingFinish(EventCbOnCapturingFinish cb);

private:
  // 静态辅助方法，便于绘制捕获过程中捕获的矩形框信息
  static void drawCaptureRectInfo(Painter *, Point &startPos,
                                  Point &currentPos);

  /**
   * 存储事件回调函数
   */
  EventCbOnCapturingFinish event_cb_on_capturing_finish_;
};
} // namespace capi
