#pragma once
#include "core/base/point.h"
#include "core/base/size.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "layer.h"

namespace capi {

class ExploreLayer: public Layer {

public:
  ExploreLayer(Image *img, const Size &canvasSize);
  /**
   * 核心绘制
   */
  void onPaint(Painter *) override;

  void onKeyPress(Key, KeyboardModifier) override;

private:
  void paintToolPane(Painter *, int mouseX, int mouseY);
  void paintToolPaneAt(Painter *, int panelX, int panelY, int mouseX, int mouseY);

private:
  Image *canvas_img_;
};

} // namespace capi
