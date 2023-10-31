#pragma once
#include "capi_core/layer/shape/shape.h"
#include "capi_core/paint/color.h"
namespace capi {
class RectShape : public Shape {
 public:
  explicit RectShape(const ShapeConfig &);
  void OnContentPaint(Painter *painter) override;
  int type() override;
};
}
