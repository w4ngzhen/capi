#pragma once

#include "shape.h"
#include "core/base/size.h"
namespace capi {
/**
 * 特殊的图形：捕获矩形图形，这个图形主要作为最底层的图形捕获
 */
class CapturedShape : public Shape {
protected:
  void OnContentPaint(Painter *painter) override;
  void OnSelectedStatusAnchorPaint(Painter *,
                                   std::function<void(Painter *, const Rect &)>) override;
public:
  int type() override;
  /**
   * CapturedShape 需要感知画布尺寸，以便绘制捕获区域之外的阴影
   */
  void SetCanvasSize(const Size &);
private:
  Size canvas_size_;
};
}