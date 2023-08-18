#pragma once

#include "../base/size.h"
#include "../base/point.h"

namespace capi {
class Canvas {
public:

  void setCursorPos(Point &);
  void setSize(Size &);
  void paint();

private:
  // 画布尺寸
  Size size_;
  // 指针位置
  // 该指针不可与cpp语言的指针混淆，
  // 而是指在画布上一个移动的指针，
  // 具体一点讲，如果在GUI下，鼠标在画布上移动，鼠标就是这个指针。
  Point cursor_pos_;
};
} // namespace capi
