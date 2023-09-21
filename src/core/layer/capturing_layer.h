#pragma once
#include "core/base/point.h"
#include "layer.h"
#include <functional>

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

  void OnPaint(Painter *) override;
  void OnMouseRelease(const Point &) override;
  void OnKeyPress(Key, KeyboardModifier) override;

  void SetLayerEventOnCapturingLayerFinishCb(LayerEventOnCapturingLayerFinishCb cb);

private:
  // 静态辅助方法，便于绘制捕获过程中捕获的矩形框信息
  static void DrawCaptureRectInfo(Painter *,
                                  Point &start_pos,
                                  Point &current_pos);

  /**
   * 存储事件回调函数
   */
  LayerEventOnCapturingLayerFinishCb layer_event_on_capturing_layer_finish_cb_;
};
} // namespace capi
