#pragma once
#include "core/base/rect.h"

namespace capi {

class Painter;
class Point;

enum TouchedArea {
  None = 0,
  Body,
  LeftTop,
  RightTop,
  LeftBottom,
  RightBottom,
};

class Shape {
public:
  virtual void onPaint(Painter *);

  Rect &body();

  [[nodiscard]] bool is_selected() const;
  virtual TouchedArea canBeTouched(Point &mousePos) const;

  void setIsSelected(bool);
  void setIsHover(bool);
protected:
  /**
   * 每个图形都有一个基于Rect的body
   */
  Rect body_;
  bool is_selected_;
  bool is_hover_;
};
}