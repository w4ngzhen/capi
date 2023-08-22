#pragma once

#include "color.h"

namespace capi {
class Brush {
public:

  Brush();

  explicit Brush(const Color &);
  void setColor(const Color &);
  Color color() const;

private:
  Color color_;
};
} // namespace capi
