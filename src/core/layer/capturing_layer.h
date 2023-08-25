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
    LayerEventOnCapturingLayerFinishCb;

class CapturingLayer : public Layer {

public:
  explicit CapturingLayer(const Size &);

  void onPaint(Painter *) override;
  void onMouseRelease(const Point &) override;
  void onKeyPress(Key, KeyboardModifier) override;

  void setLayerEventOnCapturingLayerFinishCb(LayerEventOnCapturingLayerFinishCb cb);

private:
  // 静态辅助方法，便于绘制捕获过程中捕获的矩形框信息
  static void drawCaptureRectInfo(Painter *, Point &startPos,
                                  Point &currentPos);

  /**
   * 存储事件回调函数
   */
  LayerEventOnCapturingLayerFinishCb layer_event_on_capturing_layer_finish_cb_;
};
} // namespace capi
