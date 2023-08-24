#include "captured_layer.h"
#include "core/paint/brush.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"

namespace capi {

const int CORNER_OFFSET = 5;
const int CORNER_CIRCLE_RADIUS = 3;

CapturedLayer::CapturedLayer(const Size &canvasSize)
    : Layer(canvasSize), opr_mode_(OperationMode::Normal),
      captured_rect_(Rect()) {}

void CapturedLayer::onPaint(Painter *painter) {

  painter->save();

  // 将截图矩形的周围都添加灰色蒙版
  int rX = this->captured_rect_.x();
  int rY = this->captured_rect_.y();
  int rW = this->captured_rect_.w();
  int rH = this->captured_rect_.h();

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

  Color color(0, 111, 222);
  // 增加一个边框
  painter->setPen(Pen(color, 1));
  painter->drawRect(this->captured_rect_);

  // 2. 绘制四个角的圆点
  const auto lt = math_utils::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rt = math_utils::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto lb = math_utils::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rb = math_utils::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  // 先填充圆
  painter->setBrush(Brush(color));
  painter->drawEllipse(lt);
  painter->drawEllipse(rt);
  painter->drawEllipse(lb);
  painter->drawEllipse(rb);
  // 在绘制圆的边框
  painter->setPen(Pen(Color(255, 255, 255)));
  painter->drawEllipse(lt);
  painter->drawEllipse(rt);
  painter->drawEllipse(lb);
  painter->drawEllipse(rb);
  painter->restore();
}

void CapturedLayer::onMousePress(const Point &pos) {

  Layer::onMousePress(pos);

  auto mousePos = pos;

  if (math_utils::posInEffectiveRect(mousePos, this->captured_rect_, 10)) {
    // 在有效区域内点击，则拖动rect
    this->opr_mode_ = DraggingArea;
    return;
  }
  // 否则，不属于区域拖动
  // 开始判断是否点击到了某个角
  auto effectiveRadius = CORNER_CIRCLE_RADIUS * 2;
  auto capRect = this->captured_rect_;
  auto ltRect = math_utils::getCircleRectByPoint(capRect.x() - CORNER_OFFSET,
                                                 capRect.y() - CORNER_OFFSET,
                                                 effectiveRadius);
  if (ltRect.contains(mousePos)) {
    this->opr_mode_ = DraggingLeftTop;
    return;
  }
  auto rtRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET, capRect.y() - CORNER_OFFSET,
      effectiveRadius);
  if (rtRect.contains(mousePos)) {
    this->opr_mode_ = DraggingRightTop;
    return;
  }
  auto lbRect = math_utils::getCircleRectByPoint(
      capRect.x() - CORNER_OFFSET, capRect.y() + capRect.h() + CORNER_OFFSET,
      effectiveRadius);
  if (lbRect.contains(mousePos)) {
    this->opr_mode_ = DraggingLeftBottom;
    return;
  }
  auto rbRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET,
      capRect.y() + capRect.h() + CORNER_OFFSET, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    this->opr_mode_ = DraggingRightBottom;
    return;
  }
  // 否则也不是角落点击
  this->opr_mode_ = Normal;
}
void CapturedLayer::onMouseMove(const Point &pos) {

  auto lastPos = this->mouse_current_pos_;
  auto currPos = pos;
  int dx = currPos.x() - lastPos.x();
  int dy = currPos.y() - lastPos.y();

  // 务必更新鼠标位置
  this->mouse_current_pos_ = pos;

  if (this->opr_mode_ == OperationMode::Normal) {
    return;
  }

  Rect lastCapRect = this->captured_rect_;
  if (this->opr_mode_ == OperationMode::DraggingArea) {
    // 移动捕获区域
    this->captured_rect_ = Rect(lastCapRect.x() + dx, lastCapRect.y() + dy,
                                lastCapRect.w(), lastCapRect.h());
  } else if (this->opr_mode_ == DraggingLeftTop ||
             this->opr_mode_ == DraggingRightTop ||
             this->opr_mode_ == DraggingLeftBottom ||
             this->opr_mode_ == DraggingRightBottom) {
    // 4个角拖动
    auto lt = Point(lastCapRect.x(), lastCapRect.y());
    auto rt = Point(lt.x() + lastCapRect.w(), lt.y());
    auto lb = Point(lt.x(), lt.y() + lastCapRect.h());
    auto rb = Point(rt.x(), lb.y());
    Point *draggingCor; // 正在拖动的角
    Point *fixedCor;    // 正在拖动角的对角固定不动
    switch (this->opr_mode_) {
    case DraggingLeftTop:
      draggingCor = &lt;
      fixedCor = &rb;
      break;
    case DraggingRightTop:
      draggingCor = &rt;
      fixedCor = &lb;
      break;
    case DraggingLeftBottom:
      draggingCor = &lb;
      fixedCor = &rt;
      break;
    case DraggingRightBottom:
    default:
      draggingCor = &rb;
      fixedCor = &lt;
      break;
    }
    Rect rect =
        math_utils::calcRect(draggingCor->x() + dx, draggingCor->y() + dy,
                             fixedCor->x(), fixedCor->y());
    this->captured_rect_ = rect;
  }
}
void CapturedLayer::onMouseRelease(const Point &pos) {
  switch (this->opr_mode_) {
  case Normal:
  case DraggingArea:
  case DraggingLeftTop:
  case DraggingRightTop:
  case DraggingLeftBottom:
  case DraggingRightBottom:
    this->opr_mode_ = Normal;
    break;
  case DrawingRect:
    // 用户绘制矩形以后，松开鼠标，回到启用绘制矩形状态
    this->opr_mode_ = EnableDrawRect;
    break;
  default:
    break;
  }
}

void CapturedLayer::onMouseDoubleClick(const Point &) {}

void CapturedLayer::onKeyPress(Key k, KeyboardModifier m) {
  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->event_cb_on_quit_current_layer_();
    return;
  }
}
void CapturedLayer::setCapturedRect(const Rect &rect) {
  this->captured_rect_ = rect;
}
} // namespace capi
