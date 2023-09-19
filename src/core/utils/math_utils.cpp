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

Rect getSquareByPoint(int x, int y, int size) {
  int thickness = (size - 1) / 2;
  int left = x - thickness;
  int top = y - thickness;
  int w = size;
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
Rect enlargeRect(const Rect &origin, int extendSize) {
  auto x = origin.x() - extendSize;
  auto y = origin.y() - extendSize;
  auto w = origin.w() + 2 * extendSize;
  auto h = origin.h() + 2 * extendSize;
  return {x, y, w, h};
}
bool calcCornerDragRect(const Rect &originRect, int dx, int dy, ShapePart part, Rect *targetRect) {
  if (targetRect == nullptr) {
    return false;
  }
  // 不满足四个角，直接false
  if (!checkIsCornerPart(part)) {
    return false;
  }
  // 4个角拖动
  auto lt = Point(originRect.x(), originRect.y());
  auto rt = Point(lt.x() + originRect.w(), lt.y());
  auto lb = Point(lt.x(), lt.y() + originRect.h());
  auto rb = Point(rt.x(), lb.y());
  Point *draggingCor; // 正在拖动的角
  Point *fixedCor;    // 正在拖动角的对角固定不动
  switch (part) {
    case LeftTop:draggingCor = &lt;
      fixedCor = &rb;
      break;
    case RightTop:draggingCor = &rt;
      fixedCor = &lb;
      break;
    case LeftBottom:draggingCor = &lb;
      fixedCor = &rt;
      break;
    case RightBottom: // checkIsCornerPart 判断后，这里只剩 RightBottom
    default:draggingCor = &rb;
      fixedCor = &lt;
      break;
  }
  Rect rect = calcRect(draggingCor->x() + dx, draggingCor->y() + dy,
                       fixedCor->x(), fixedCor->y());
  targetRect->setX(rect.x());
  targetRect->setY(rect.y());
  targetRect->setW(rect.w());
  targetRect->setH(rect.h());
  return true;
}
bool checkIsCornerPart(ShapePart part) {
  return part == LeftTop || part == RightTop || part == LeftBottom || part == RightBottom;
}
} // namespace capi::math_utils

