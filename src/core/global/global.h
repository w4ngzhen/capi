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

/**
 * Key枚举
 */
enum Key {
  // Unicode Basic Latin block (0x00-0x7f)
  Key_Space = 0x20,
  Key_Any = Key_Space,
  Key_Exclam = 0x21,
  Key_QuoteDbl = 0x22,
  Key_NumberSign = 0x23,
  Key_Dollar = 0x24,
  Key_Percent = 0x25,
  Key_Ampersand = 0x26,
  Key_Apostrophe = 0x27,
  Key_ParenLeft = 0x28,
  Key_ParenRight = 0x29,
  Key_Asterisk = 0x2a,
  Key_Plus = 0x2b,
  Key_Comma = 0x2c,
  Key_Minus = 0x2d,
  Key_Period = 0x2e,
  Key_Slash = 0x2f,
  Key_0 = 0x30,
  Key_1 = 0x31,
  Key_2 = 0x32,
  Key_3 = 0x33,
  Key_4 = 0x34,
  Key_5 = 0x35,
  Key_6 = 0x36,
  Key_7 = 0x37,
  Key_8 = 0x38,
  Key_9 = 0x39,
  Key_Colon = 0x3a,
  Key_Semicolon = 0x3b,
  Key_Less = 0x3c,
  Key_Equal = 0x3d,
  Key_Greater = 0x3e,
  Key_Question = 0x3f,
  Key_At = 0x40,
  Key_A = 0x41,
  Key_B = 0x42,
  Key_C = 0x43,
  Key_D = 0x44,
  Key_E = 0x45,
  Key_F = 0x46,
  Key_G = 0x47,
  Key_H = 0x48,
  Key_I = 0x49,
  Key_J = 0x4a,
  Key_K = 0x4b,
  Key_L = 0x4c,
  Key_M = 0x4d,
  Key_N = 0x4e,
  Key_O = 0x4f,
  Key_P = 0x50,
  Key_Q = 0x51,
  Key_R = 0x52,
  Key_S = 0x53,
  Key_T = 0x54,
  Key_U = 0x55,
  Key_V = 0x56,
  Key_W = 0x57,
  Key_X = 0x58,
  Key_Y = 0x59,
  Key_Z = 0x5a,
  Key_BracketLeft = 0x5b,
  Key_Backslash = 0x5c,
  Key_BracketRight = 0x5d,
  Key_AsciiCircum = 0x5e,
  Key_Underscore = 0x5f,
  Key_QuoteLeft = 0x60,
  Key_BraceLeft = 0x7b,
  Key_Bar = 0x7c,
  Key_BraceRight = 0x7d,
  Key_AsciiTilde = 0x7e,

  // The rest of the Unicode values are skipped here,
  // so that we can represent them along with Qt::Keys
  // in the same data type. The maximum Unicode value
  // is 0x0010ffff, so we start our custom keys at
  // 0x01000000 to not clash with the Unicode values,
  // but still give plenty of room to grow.

  Key_Escape = 0x01000000, // misc keys
  Key_Tab = 0x01000001,
  Key_Backtab = 0x01000002,
  Key_Backspace = 0x01000003,
  Key_Return = 0x01000004,
  Key_Enter = 0x01000005,
  Key_Insert = 0x01000006,
  Key_Delete = 0x01000007,
  Key_Pause = 0x01000008,
  Key_Print = 0x01000009, // print screen
  Key_SysReq = 0x0100000a,
  Key_Clear = 0x0100000b,
  Key_Home = 0x01000010, // cursor movement
  Key_End = 0x01000011,
  Key_Left = 0x01000012,
  Key_Up = 0x01000013,
  Key_Right = 0x01000014,
  Key_Down = 0x01000015,
  Key_PageUp = 0x01000016,
  Key_PageDown = 0x01000017,
  Key_Shift = 0x01000020, // modifiers
  Key_Control = 0x01000021,
  Key_Meta = 0x01000022,
  Key_Alt = 0x01000023,
  Key_CapsLock = 0x01000024,
  Key_NumLock = 0x01000025,
  Key_ScrollLock = 0x01000026,
  Key_F1 = 0x01000030, // function keys
  Key_F2 = 0x01000031,
  Key_F3 = 0x01000032,
  Key_F4 = 0x01000033,
  Key_F5 = 0x01000034,
  Key_F6 = 0x01000035,
  Key_F7 = 0x01000036,
  Key_F8 = 0x01000037,
  Key_F9 = 0x01000038,
  Key_F10 = 0x01000039,
  Key_F11 = 0x0100003a,
  Key_F12 = 0x0100003b,
};

/**
 * 按键modifier
 */
enum KeyboardModifier {
  NoModifier = 0x00000000,
  ShiftModifier = 0x02000000,
  ControlModifier = 0x04000000,
  AltModifier = 0x08000000,
  MetaModifier = 0x10000000,
  KeypadModifier = 0x20000000,
  GroupSwitchModifier = 0x40000000,
  // Do not extend the mask to include 0x01000000
  KeyboardModifierMask = 0xfe000000
};

} // namespace capi
