#include "captured_layer.h"
#include "core/paint/brush.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"
#include "core/event/captured_image_save_event.h"

namespace capi {

const int CORNER_OFFSET = 5;
const int CORNER_CIRCLE_RADIUS = 3;

CapturedLayer::CapturedLayer(const Size &canvasSize)
    : Layer(canvasSize), dragging_part_(ShapePart::None),
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
    this->dragging_part_ = Body;
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
    this->dragging_part_ = LeftTop;
    return;
  }
  auto rtRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET, capRect.y() - CORNER_OFFSET,
      effectiveRadius);
  if (rtRect.contains(mousePos)) {
    this->dragging_part_ = RightTop;
    return;
  }
  auto lbRect = math_utils::getCircleRectByPoint(
      capRect.x() - CORNER_OFFSET, capRect.y() + capRect.h() + CORNER_OFFSET,
      effectiveRadius);
  if (lbRect.contains(mousePos)) {
    this->dragging_part_ = LeftBottom;
    return;
  }
  auto rbRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET,
      capRect.y() + capRect.h() + CORNER_OFFSET, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    this->dragging_part_ = RightBottom;
    return;
  }
  // 否则也不是角落点击
  this->dragging_part_ = None;
}
void CapturedLayer::onMouseMove(const Point &pos) {

  auto lastPos = this->mouse_current_pos_;
  // 务必更新鼠标位置
  this->mouse_current_pos_ = pos;

  // 计算偏移
  int dx = pos.x() - lastPos.x();
  int dy = pos.y() - lastPos.y();

  if (this->dragging_part_ == None) {
    return;
  }

  Rect lastCapRect = this->captured_rect_;

  if (this->dragging_part_ == Body) {
    // 移动整个区域
    this->captured_rect_ = Rect(lastCapRect.x() + dx, lastCapRect.y() + dy,
                                lastCapRect.w(), lastCapRect.h());
  } else if (this->dragging_part_ == LeftTop ||
      this->dragging_part_ == RightTop ||
      this->dragging_part_ == LeftBottom ||
      this->dragging_part_ == RightBottom) {
    // 4个角拖动
    auto lt = Point(lastCapRect.x(), lastCapRect.y());
    auto rt = Point(lt.x() + lastCapRect.w(), lt.y());
    auto lb = Point(lt.x(), lt.y() + lastCapRect.h());
    auto rb = Point(rt.x(), lb.y());
    Point *draggingCor; // 正在拖动的角
    Point *fixedCor;    // 正在拖动角的对角固定不动
    switch (this->dragging_part_) {
      case LeftTop:draggingCor = &lt;
        fixedCor = &rb;
        break;
      case RightTop:draggingCor = &rt;
        fixedCor = &lb;
        break;
      case LeftBottom:draggingCor = &lb;
        fixedCor = &rt;
        break;
      case RightBottom:
      default: draggingCor = &rb;
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
  switch (this->dragging_part_) {
    case None:
    case Body:
    case LeftTop:
    case RightTop:
    case LeftBottom:
    case RightBottom:this->dragging_part_ = None;
      break;
    default:break;
  }
}

void CapturedLayer::onMouseDoubleClick(const Point &) {
  // 准备构造captured canvas_image 保存事件
  // 暂时只支持保存到粘贴板
  CapturedImageSaveEvent ev(this->captured_rect_, SaveMode::Clipboard);
  this->layer_event_on_captured_layer_image_save_cb_(&ev);
}

void CapturedLayer::onKeyPress(Key k, KeyboardModifier m) {
  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->layer_event_on_quit_current_layer_cb_();
    return;
  }
}
void CapturedLayer::setCapturedRect(const Rect &rect) {
  this->captured_rect_ = rect;
  this->dragging_part_ = None;
}

void CapturedLayer::setLayerEventOnCapturedLayerImageSaveCb(LayerEventOnCapturedLayerImageSaveCb cb) {
  this->layer_event_on_captured_layer_image_save_cb_ = std::move(cb);
}
} // namespace capi
