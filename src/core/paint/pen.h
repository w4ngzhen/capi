#pragma once

#include "color.h"
#include "core/global/global.h"

namespace capi {

class Pen {
public:
  Pen();
  explicit Pen(const Color &);
  Pen(const Color &, int width);
  Pen(int width, const Color &, PenStyle);
  void SetWidth(int w);
  void SetColor(const Color &);
  void SetPenStyle(PenStyle);

  [[nodiscard]] int width() const;
  [[nodiscard]] Color color() const;
  [[nodiscard]] PenStyle pen_style() const;

private:
  int width_;
  Color color_;
  PenStyle pen_style_;
};
} // namespace capi
