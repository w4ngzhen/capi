#pragma once
#include "shape.h"
#include "core/paint/color.h"
namespace capi {
class RectShape : public Shape {
public:
  explicit RectShape(const ShapeConfig &);
  void onContentPaint(Painter *painter) override;
};
}
