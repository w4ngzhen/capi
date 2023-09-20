#pragma once

#include "shape.h"
#include "core/base/size.h"
namespace capi {
/**
 * 特殊的图形：捕获矩形图形，这个图形主要作为最底层的图形捕获
 */
class CapturedShape : public Shape {
protected:
  void onContentPaint(Painter *painter) override;
public:
  int type() override;
  /**
   * CapturedShape 需要感知画布尺寸，以便绘制捕获区域之外的阴影
   */
  void setCanvasSize(const Size &);
private:
  Size canvas_size_;
};
}