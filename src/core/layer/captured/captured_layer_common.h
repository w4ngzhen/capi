#pragma once

namespace capi {
/**
 * 标识对于一个图形的某个部分
 * 该枚举主要用于：
 * 1)CapturedLayer中的captured rect（算特殊的矩形shape）的drag、resize；
 * 2)CapturedLayer层中某个Shape的drag、resize
 */
enum ShapePart {
  None = 0,

  Body = 0x01,
  LeftTop = 0x10,
  RightTop = 0x12,
  LeftBottom = 0x14,
  RightBottom = 0x16,
  /*
   * 特例：如果Shape是一个线，拖动开始端点
   */
  LineStart = 0x20,
  /*
  * 特例：如果Shape是一个线，拖动结束端点
  */
  LineEnd = 0x22
};
}