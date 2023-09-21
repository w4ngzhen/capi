#pragma once
#include "shape.h"
#include "core/paint/color.h"
namespace capi {
class RectShape : public Shape {
public:
  explicit RectShape(const ShapeConfig &);
  void OnContentPaint(Painter *painter) override;
  int type() override;
};
}
