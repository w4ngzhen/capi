#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "../layer.h"
#include "operation_mode.h"
#include <cwchar>

namespace capi {

/**
 * 事件回调EventCb
 * 区域捕获完成，并处理完成事件
 */
typedef std::function<void(const Rect &capturedRect)>
    EventCbOnCapturedFinish;

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

private:
  Rect captured_rect_;
  OperationMode opr_mode_;
};
} // namespace capi
