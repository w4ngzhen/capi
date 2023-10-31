#pragma once
#include "capi_core/layer/shape/shape.h"
#include "capi_core/paint/color.h"
namespace capi {
class EllipseShape : public Shape {
 public:
  explicit EllipseShape(const ShapeConfig &);
  void OnContentPaint(Painter *painter) override;
  int type() override;
};
}
