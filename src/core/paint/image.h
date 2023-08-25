#pragma once

#include "color.h"
#include "core/base/rect.h"
#include "core/base/size.h"

namespace capi {
class Image {
public:
  virtual Color colorAt(int x, int y) = 0;
  virtual Size size() = 0;
};
} // namespace capi
