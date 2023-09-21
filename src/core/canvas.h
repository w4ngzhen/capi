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
class CapturedImageSaveEvent;

/**
 * 画布退出事件
 */
typedef std::function<void()> CanvasEventOnQuitCb;
/**
 * 画布图片保存事件
 */
typedef std::function<void(const CapturedImageSaveEvent *)> CanvasEventOnImageSaveCb;

class Canvas {

public:
  explicit Canvas(Image *canvasImg);
  Image *canvas_image();
  Size size();

  void OnMouseMove(const Point &);
  void OnMousePress(const Point &);
  void OnMouseRelease(const Point &);
  void OnMouseDoubleClick(const Point &);
  void OnResize(const Size &);
  void OnKeyPress(Key, KeyboardModifier);
  void OnPaint(Painter *);

  void HandleLayerEventOnCapturingLayerFinish(bool sizeValid, const Rect &capturedRect);
  void HandleLayerEventOnLayerQuitCurrentLayer(CanvasStatus status);
  void HandleLayerEventOnCapturedLayerImageSave(const CapturedImageSaveEvent *);

  void SetCanvasEventOnQuitCb(CanvasEventOnQuitCb);
  void SetCanvasEventOnImageSaveCb(CanvasEventOnImageSaveCb);

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
  CanvasEventOnQuitCb canvas_event_on_quit_cb_;
  CanvasEventOnImageSaveCb canvas_event_on_image_save_cb_;
};
} // namespace capi
