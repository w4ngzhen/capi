#include "captured_shape.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"
namespace capi {

/**
 * CapturedShape的type类型保留为0xFFFF
 * @return
 */
int CapturedShape::type() {
  return 0xFFFF;
}

void capi::CapturedShape::onContentPaint(Painter *painter) {
  painter->save();
  const auto &contentRect = this->content_rect();

  // 对于捕获图形，永远其矩形，且相关绘图配置固定
  painter->setPen(Pen(Color(0, 111, 222)));
  painter->drawRect(contentRect);
  painter->restore();
}

}

