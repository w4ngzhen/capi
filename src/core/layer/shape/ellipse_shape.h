#pragma once
#include "shape.h"
#include "core/paint/color.h"
namespace capi {
class EllipseShape : public Shape {
public:
  explicit EllipseShape(const ShapeConfig &);
  void OnContentPaint(Painter *painter) override;
  int type() override;
};
}
