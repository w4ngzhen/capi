#include "capi_core/base/point.h"
#include "capi_core/base/rect.h"
#include "capi_core/utils/math_utils.h"

#include <cmath>

namespace capi::math_utils {

Rect CalcRect(int x1, int y1, int x2, int y2) {

  int x = fmin(x1, x2);
  int y = fmin(y1, y2);
  int w = abs(x1 - x2);
  int h = abs(y1 - y2);
  return {x, y, w, h};
}

Rect GetSquareByPoint(int x, int y, int size) {
  int thickness = (size - 1) / 2;
  int left = x - thickness;
  int top = y - thickness;
  int w = size;
  int h = w;
  return {left, top, w, h};
}

bool CheckPosInEffectiveRect(const Point &pos, const Rect &rect,
                             const int border_width) {

  if (border_width <= 0) {
    return rect.Contains(pos);
  }
  Rect _rect(rect.x() + border_width, rect.y() + border_width,
             rect.w() - border_width * 2, rect.h() - border_width * 2);
  return _rect.Contains(pos);
}
Rect EnlargeRect(const Rect &origin, int extend_size) {
  auto x = origin.x() - extend_size;
  auto y = origin.y() - extend_size;
  auto w = origin.w() + 2 * extend_size;
  auto h = origin.h() + 2 * extend_size;
  return {x, y, w, h};
}
bool CalcCornerDragRect(const Rect &origin_rect, int dx, int dy, ShapePart part, Rect *target_rect) {
  if (target_rect == nullptr) {
    return false;
  }
  // 不满足四个角，直接false
  if (!CheckIsCornerPart(part)) {
    return false;
  }
  // 4个角拖动
  auto lt = Point(origin_rect.x(), origin_rect.y());
  auto rt = Point(lt.x() + origin_rect.w(), lt.y());
  auto lb = Point(lt.x(), lt.y() + origin_rect.h());
  auto rb = Point(rt.x(), lb.y());
  Point *dragging_cor; // 正在拖动的角
  Point *fixed_cor;    // 正在拖动角的对角固定不动
  switch (part) {
    case LeftTop:dragging_cor = &lt;
      fixed_cor = &rb;
      break;
    case RightTop:dragging_cor = &rt;
      fixed_cor = &lb;
      break;
    case LeftBottom:dragging_cor = &lb;
      fixed_cor = &rt;
      break;
    case RightBottom: // CheckIsCornerPart 判断后，这里只剩 RightBottom
    default:dragging_cor = &rb;
      fixed_cor = &lt;
      break;
  }
  Rect rect = CalcRect(dragging_cor->x() + dx, dragging_cor->y() + dy,
                       fixed_cor->x(), fixed_cor->y());
  target_rect->SetX(rect.x());
  target_rect->SetY(rect.y());
  target_rect->SetW(rect.w());
  target_rect->SetH(rect.h());
  return true;
}
bool CheckIsCornerPart(ShapePart part) {
  return part == LeftTop || part == RightTop || part == LeftBottom || part == RightBottom;
}
} // namespace capi::math_utils

