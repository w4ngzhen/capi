#pragma once
#include "capi_core/layer/shape/shape.h"
namespace capi {
class LineShape : public Shape {
public:
  explicit LineShape(const ShapeConfig &);
protected:
  void OnContentPaint(Painter *painter) override;
public:
  [[nodiscard]] bool is_line_shape() const override;
  int type() override;
};
}
