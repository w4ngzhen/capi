#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "../layer.h"
#include "core/event/captured_image_save_event.h"
#include "captured_layer_common.h"

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
  /**
   * 捕获的区域
   */
  Rect captured_rect_;
  /**
   * 描述当前正在拖拽 captured_rect_ 哪个位置
   */
  ShapePart dragging_part_;
  /**
   * 捕获完成、处理后的保存回调
   */
  LayerEventOnCapturedLayerImageSaveCb layer_event_on_captured_layer_image_save_cb_;
};
} // namespace capi
