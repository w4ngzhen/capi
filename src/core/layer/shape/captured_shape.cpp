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

void capi::CapturedShape::OnContentPaint(Painter *painter) {
  painter->Save();
  const auto &content_rect = this->content_rect();

  // 将截图矩形的周围都添加灰色蒙版
  int rx = content_rect.x();
  int ry = content_rect.y();
  int rw = content_rect.w();
  int rh = content_rect.h();

  Size canvas_size = this->canvas_size_;
  int canvas_w = canvas_size.w();
  int canvas_h = canvas_size.h();

  Rect left_mask_rect(0, 0, rx, canvas_h);
  Rect right_mask_rect(rx + rw, 0, canvas_w - (rx + rw), canvas_h);
  Rect top_mask_rect(rx, 0, rw, ry);
  Rect bottom_mask_rect(rx, (ry + rh), rw, canvas_h - (ry + rh));

  Brush gray_brush(Color(0, 0, 0, 90)); // 50% Alpha的灰色
  painter->FillRect(left_mask_rect, gray_brush);
  painter->FillRect(right_mask_rect, gray_brush);
  painter->FillRect(top_mask_rect, gray_brush);
  painter->FillRect(bottom_mask_rect, gray_brush);

  // 对于捕获图形，永远其矩形，且相关绘图配置固定
  painter->SetPen(Pen(Color(0, 111, 222)));
  painter->DrawRect(content_rect);
  painter->Restore();
}
void CapturedShape::SetCanvasSize(const Size &canvas_size) {
  canvas_size_ = canvas_size;
}
void CapturedShape::OnSelectedStatusAnchorPaint(
    Painter *painter,
    std::function<void(Painter *, const Rect &)>) {
  // CapturedShape 的锚点绘制，使用圆形绘制
  std::function<void(Painter *, const Rect &)> cb = [](auto *p, auto &anchor_rect) {
    // 先填充正方形
    p->SetBrush(Brush(Color(0, 111, 222)));
    p->DrawEllipse(anchor_rect);
    // 在绘制正方形边框
    p->SetPen(Pen(Color(255, 255, 255)));
    p->DrawEllipse(anchor_rect);
  };
  Shape::OnSelectedStatusAnchorPaint(painter, cb);
}

}

