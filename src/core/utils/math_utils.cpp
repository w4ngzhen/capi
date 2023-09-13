#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"
#include "math_utils.h"

#include <cmath>

namespace capi::math_utils {

Rect calcRect(int x1, int y1, int x2, int y2) {

  int x = fmin(x1, x2);
  int y = fmin(y1, y2);
  int w = abs(x1 - x2);
  int h = abs(y1 - y2);
  return {x, y, w, h};
}

Rect getCircleRectByPoint(int x, int y, int radius) {
  int left = x - radius;
  int top = y - radius;
  int w = radius * 2 + 1;
  int h = w;
  return {left, top, w, h};
}

bool posInEffectiveRect(const Point &pos, const Rect &rect,
                        const int borderWidth) {

  if (borderWidth <= 0) {
    return rect.contains(pos);
  }
  Rect _rect(rect.x() + borderWidth, rect.y() + borderWidth,
             rect.w() - borderWidth * 2, rect.h() - borderWidth * 2);
  return _rect.contains(pos);
}
Rect math_utils::enlargeRect(const Rect &origin, int extendSize) {
  auto x = origin.x() - extendSize;
  auto y = origin.y() - extendSize;
  auto w = origin.w() + 2 * extendSize;
  auto h = origin.h() + 2 * extendSize;
  return {x, y, w, h};
}
} // namespace capi::math_utils

