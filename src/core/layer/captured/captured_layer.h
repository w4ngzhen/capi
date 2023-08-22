#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "../layer.h"
#include "operation_mode.h"

namespace capi {

class CapturedLayer: public Layer {

public:
  explicit CapturedLayer(const Size &);
  void onPaint(Painter *) override;

  void onMousePress(const Point &) override;
  void onMouseMove(const Point &) override;
  void onMouseRelease(const Point &) override;

private:
  Rect captured_rect_;
  OperationMode opr_mode_;
};
} // namespace capi
