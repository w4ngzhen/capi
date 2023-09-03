#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "../layer.h"
#include "operation_mode.h"
#include "core/event/captured_image_save_event.h"

namespace capi {

/**
 * 事件回调EventCb
 * 区域捕获完成，并处理完成事件
 */
typedef std::function<void(const CapturedImageSaveEvent *)>
    LayerEventOnCapturedLayerImageSaveCb;

class CapturedLayer : public Layer {

public:
  explicit CapturedLayer(const Size &);
  void onPaint(Painter *) override;

  void onMousePress(const Point &) override;
  void onMouseMove(const Point &) override;
  void onMouseRelease(const Point &) override;
  void onMouseDoubleClick(const Point &) override;
  void onKeyPress(Key, KeyboardModifier) override;

  void setCapturedRect(const Rect &);

  /**
   * 设置图片捕获处理后的保存事件
   */
  void setLayerEventOnCapturedLayerImageSaveCb(LayerEventOnCapturedLayerImageSaveCb cb);

private:
  Rect captured_rect_;
  OperationMode opr_mode_;
  LayerEventOnCapturedLayerImageSaveCb layer_event_on_captured_layer_image_save_cb_;
};
} // namespace capi
