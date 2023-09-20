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

  // 将截图矩形的周围都添加灰色蒙版
  int rX = contentRect.x();
  int rY = contentRect.y();
  int rW = contentRect.w();
  int rH = contentRect.h();

  Size canvasSize = this->canvas_size_;
  int canvasW = canvasSize.w();
  int canvasH = canvasSize.h();

  Rect leftMaskRect(0, 0, rX, canvasH);
  Rect rightMaskRect(rX + rW, 0, canvasW - (rX + rW), canvasH);
  Rect topMaskRect(rX, 0, rW, rY);
  Rect bottomMaskRect(rX, (rY + rH), rW, canvasH - (rY + rH));

  Brush grayBrush(Color(0, 0, 0, 50)); // 50% Alpha的灰色
  painter->fillRect(leftMaskRect, grayBrush);
  painter->fillRect(rightMaskRect, grayBrush);
  painter->fillRect(topMaskRect, grayBrush);
  painter->fillRect(bottomMaskRect, grayBrush);

  // 对于捕获图形，永远其矩形，且相关绘图配置固定
  painter->setPen(Pen(Color(0, 111, 222)));
  painter->drawRect(contentRect);
  painter->restore();
}
void CapturedShape::setCanvasSize(const Size &canvasSize) {
  canvas_size_ = canvasSize;
}
void CapturedShape::onSelectedStatusAnchorPaint(
    Painter *painter,
    std::function<void(Painter *, const Rect &)>) {
  // CapturedShape 的锚点绘制，使用圆形绘制
  std::function<void(Painter *, const Rect &)> cb = [](auto *p, auto &anchor_rect) {
    // 先填充正方形
    p->setBrush(Brush(Color(0, 111, 222)));
    p->drawEllipse(anchor_rect);
    // 在绘制正方形边框
    p->setPen(Pen(Color(255, 255, 255)));
    p->drawEllipse(anchor_rect);
  };
  Shape::onSelectedStatusAnchorPaint(painter, cb);
}

}

