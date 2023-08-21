#pragma once

#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"

namespace capi {
/**
 * 画笔样式
 **/
enum class PenStyle {
  SolidLine = 0,
  DotLine,
  DashLine,
};

enum AlignMode {
  None = 0,
  Center = 0x01,
  Left = 0x02,
  Right = 0x03,
  VCenter = 0x04,
  VLeft = 0x05,
  VRight = 0x06,
};

enum Key {
  Key_Up,
  Key_Down,
  Key_Left,
  Key_Right,
  Key_Esc,
};

} // namespace capi
