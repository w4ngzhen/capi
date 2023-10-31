#pragma once
#include "capi_core/base/point.h"
#include "capi_core/base/size.h"
#include "capi_core/paint/image.h"
#include "capi_core/paint/painter.h"
#include "capi_core/layer/layer.h"

namespace capi {

class ExploreLayer : public Layer {

public:
  ExploreLayer(Image *img, const Size &canvas_size);
  /**
   * 核心绘制
   */
  void OnPaint(Painter *) override;

  void OnKeyPress(Key, KeyboardModifier) override;

private:
  void paintToolPane(Painter *, int mouse_x, int mouse_y);
  void paintToolPaneAt(Painter *, int panel_x, int panel_y, int mouse_x, int mouse_y);

private:
  Image *canvas_img_;
};

} // namespace capi
