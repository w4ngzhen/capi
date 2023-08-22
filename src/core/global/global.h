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

/**
 * 布局标识，这里直接复用Qt的枚举AlignmentFlag
 * qnamespace.h
 */
enum AlignFlag {
  AlignLeft = 0x0001,
  AlignLeading = AlignLeft,
  AlignRight = 0x0002,
  AlignTrailing = AlignRight,
  AlignHCenter = 0x0004,
  AlignJustify = 0x0008,
  AlignAbsolute = 0x0010,
  AlignHorizontal_Mask =
      AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,

  AlignTop = 0x0020,
  AlignBottom = 0x0040,
  AlignVCenter = 0x0080,
  AlignBaseline = 0x0100,
  // Note that 0x100 will clash with Qt::TextSingleLine = 0x100 due to what the
  // comment above this enum declaration states. However, since
  // Qt::AlignBaseline is only used by layouts, it doesn't make sense to pass
  // Qt::AlignBaseline to QPainter::drawText(), so there shouldn't really be any
  // ambiguity between the two overlapping enum values.
  AlignVertical_Mask = AlignTop | AlignBottom | AlignVCenter | AlignBaseline,

  AlignCenter = AlignVCenter | AlignHCenter
};

enum Key {
  Key_Up,
  Key_Down,
  Key_Left,
  Key_Right,
  Key_Esc,
};

} // namespace capi
