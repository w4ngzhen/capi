#pragma once

#include "color.h"
#include "core/global/global.h"

namespace capi {

class Pen {
public:
  Pen();
  Pen(const Color &);
  Pen(const Color &, int width);
  Pen(int width, const Color &, PenStyle);
  void setWidth(int w);
  void setColor(const Color &);
  void setPenStyle(PenStyle);

  int width() const;
  Color color() const;
  PenStyle pen_style() const;

private:
  int width_;
  Color color_;
  PenStyle pen_style_;
};
} // namespace capi
