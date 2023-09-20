#pragma once

#include "shape.h"
namespace capi {
/**
 * 特殊的图形：捕获矩形图形，这个图形主要作为最底层的图形捕获
 */
class CapturedShape : public Shape {
protected:
  void onContentPaint(Painter *painter) override;
public:
  int type() override;
};
}