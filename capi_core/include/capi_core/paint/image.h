#pragma once

#include "capi_core/paint/color.h"
#include "capi_core/base/rect.h"
#include "capi_core/base/size.h"

namespace capi {
class Image {
public:
  virtual Color GetColorAt(int x, int y) = 0;
  virtual Size size() = 0;
};
} // namespace capi
