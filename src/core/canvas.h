#pragma once

#include "canvas_status.h"
#include "core/base/point.h"
#include "core/base/size.h"
#include "core/global/global.h"
#include "core/paint/image.h"
#include <functional>

namespace capi {

class Painter;
class ExploreLayer;
class CapturingLayer;
class CapturedLayer;
class CanvasCapturedImageSaveEvent;

/**
 * 画布退出事件
 */
typedef std::function<void()> OnCanvasQuitCb;
/**
 * 画布图片保存事件
 */
typedef std::function<void(const CanvasCapturedImageSaveEvent *)> OnCanvasImageSaveCb;

class Canvas {

public:
  explicit Canvas(Image *canvasImg);

  void onMouseMove(const Point &);
  void onMousePress(const Point &);
  void onMouseRelease(const Point &);
  void onMouseDoubleClick(const Point &);
  void onResize(const Size &);
  void onKeyPress(Key, KeyboardModifier);
  void onPaint(Painter *);

  void eventCbHandleOnCapturingFinish(bool sizeValid, const Rect &capturedRect);
  void eventCbHandleOnQuitCurrentLayer(CanvasStatus status);

  void setOnCanvasQuitCb(OnCanvasQuitCb);
  void setOnCanvasImageSaveCb(OnCanvasImageSaveCb);

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
   * 画布自身事件
   */
  OnCanvasQuitCb on_canvas_quit_cb_;
  OnCanvasImageSaveCb on_canvas_image_save_cb_;
};
} // namespace capi
