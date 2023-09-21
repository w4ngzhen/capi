#pragma once

#include "color.h"

namespace capi {
class Brush {
public:

  Brush();

  explicit Brush(const Color &);
  void SetColor(const Color &color);
  [[nodiscard]] Color color() const;

private:
  Color color_;
};
} // namespace capi
