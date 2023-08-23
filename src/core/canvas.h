#pragma once

#include "core/base/point.h"
#include "core/base/size.h"
#include "canvas_status.h"
#include "core/global/global.h"
#include "core/paint/image.h"

namespace capi {

class Painter;
class ExploreLayer;
class CapturingLayer;
class CapturedLayer;

class Canvas {

public:
  explicit Canvas(Image *canvasImg);

  void onMouseMove(const Point &);
  void onMousePress(const Point &);
  void onMouseRelease(const Point &);
  void onMouseDoubleClick(const Point &);
  void onResize(const Size &);
  void onKeyPress(Key);
  void onPaint(Painter *);

  void eventCbHandleOnCapturingFinish(bool sizeValid, const Rect &capturedRect);

  ~Canvas();

private:
  // 画布尺寸
  Size size_{};
  // 指针位置
  // 该指针不可与cpp语言的指针混淆，
  // 而是指在画布上一个移动的指针，
  // 具体一点讲，如果在GUI下，鼠标在画布上移动，鼠标就是这个指针。
  Point cursor_pos_;

  Image *canvas_img_;

  CanvasStatus status_;
  /**
   * 基础的三层
   */
  ExploreLayer *explore_layer_;
  CapturingLayer *capturing_layer_;
  CapturedLayer *captured_layer_;
  /**
   * TODO: 绘画操作层
   */
};
} // namespace capi
