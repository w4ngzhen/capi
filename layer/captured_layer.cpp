#include "captured_layer.h"
#include "helper/math_helper.h"
#include "helper/paint_helper.h"

#include <QPainter>
#include <QRect>

const int CORNER_OFFSET = 5;
const int CORNER_CIRCLE_RADIUS = 3;

CapturedLayer::CapturedLayer(QImage *screenPic, QSize screenSize,
                             QSize screenScale)
    : screen_pic_(screenPic), screen_size_(screenSize),
      screen_scale_(screenScale), is_area_dragging_(false),
      mouse_last_pos_(QPoint()) {}

void CapturedLayer::setCapturedRect(const QRect &rect) {
  this->captured_rect_ = QRect(rect);
}

void CapturedLayer::paint(QPainter &painter) {
  painter.save();

  // 将截图矩形的周围都添加灰色蒙版
  int rX = this->captured_rect_.x();
  int rY = this->captured_rect_.y();
  int rW = this->captured_rect_.width();
  int rH = this->captured_rect_.height();

  QSize screenSize = this->screen_size_;
  int screenW = screenSize.width();
  int screenH = screenSize.height();

  QRect leftMaskRect(0, 0, rX, screenH);
  QRect rightMaskRect(rX + rW, 0, screenW - (rX + rW), screenH);
  QRect topMaskRect(rX, 0, rW, rY);
  QRect bottomMaskRect(rX, (rY + rH), rW, screenH - (rY + rH));

  QBrush grayBrush = QBrush(QColor(0, 0, 0, 50)); // 50% Alpha的灰色
  painter.fillRect(leftMaskRect, grayBrush);
  painter.fillRect(rightMaskRect, grayBrush);
  painter.fillRect(topMaskRect, grayBrush);
  painter.fillRect(bottomMaskRect, grayBrush);

  QColor color(0, 111, 222);
  // 增加一个边框
  painter.setPen(QPen(color, 1));
  painter.drawRect(this->captured_rect_);

  // 2. 绘制四个角的圆点
  const auto lt = paint_helper::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rt = paint_helper::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto lb = paint_helper::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rb = paint_helper::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  painter.setBrush(color);
  painter.drawEllipse(lt);
  painter.drawEllipse(rt);
  painter.drawEllipse(lb);
  painter.drawEllipse(rb);

  painter.restore();
}

void CapturedLayer::mouseDoubleClickEvent(QMouseEvent *) {
  auto logicRect = this->captured_rect_;
  auto realRect = paint_helper::rectLogicPixelToRealPixel(
      logicRect, this->screen_scale_.width(), this->screen_scale_.height());
  emit saveCapturedRectSignal(logicRect, realRect,
                              CapturedRectSaveType::ToClipboard);
}

void CapturedLayer::mouseMoveEvent(QMouseEvent *event) {

  if (!this->is_area_dragging_ && this->dragging_corner_ < 0) {
    // 非整体移动，或不是在拖动四个角
    return;
  }

  auto currPos = event->pos();
  auto lastPos = this->mouse_last_pos_;

  // 更新鼠标位置，为下一次move作准备
  this->mouse_last_pos_ = currPos;

  // 计算偏移
  int dx = currPos.x() - lastPos.x();
  int dy = currPos.y() - lastPos.y();

  QRect lastCapRect = this->captured_rect_;
  if (this->is_area_dragging_) {
    // 移动捕获区域
    this->captured_rect_ = QRect(lastCapRect.x() + dx, lastCapRect.y() + dy,
                                 lastCapRect.width(), lastCapRect.height());
  } else {
    // 4个角拖动
    auto lt = QPoint(lastCapRect.x(), lastCapRect.y());
    auto rt = QPoint(lt.x() + lastCapRect.width(), lt.y());
    auto lb = QPoint(lt.x(), lt.y() + lastCapRect.height());
    auto rb = QPoint(rt.x(), lb.y());
    QPoint *draggingCor; // 正在拖动的角
    QPoint *fixedCor;    // 正在拖动角的对角固定不动
    switch (this->dragging_corner_) {
    case 0:
      draggingCor = &lt;
      fixedCor = &rb;
      break;
    case 1:
      draggingCor = &rt;
      fixedCor = &lb;
      break;
    case 2:
      draggingCor = &lb;
      fixedCor = &rt;
      break;
    case 3:
    default:
      draggingCor = &rb;
      fixedCor = &lt;
      break;
    }
    QRect rect =
        math_helper::calcRect(draggingCor->x() + dx, draggingCor->y() + dy,
                              fixedCor->x(), fixedCor->y());
    this->captured_rect_ = rect;
  }

  // 外部需要update
}

void CapturedLayer::mousePressEvent(QMouseEvent *event) {
  auto mousePos = event->pos();
  this->mouse_last_pos_ = mousePos;

  if (math_helper::posInEffectiveRect(mousePos, this->captured_rect_, 10)) {
    // 在有效区域内点击，则拖动rect
    this->is_area_dragging_ = true;
    return;
  }
  // 否则，不属于区域拖动
  this->is_area_dragging_ = false;
  // 开始判断是否点击到了某个角
  auto effectiveRadius = CORNER_CIRCLE_RADIUS * 2;
  auto capRect = this->captured_rect_;
  auto ltRect = paint_helper::getCircleRectByPoint(capRect.x() - CORNER_OFFSET,
                                                   capRect.y() - CORNER_OFFSET,
                                                   effectiveRadius);
  if (ltRect.contains(mousePos)) {
    this->dragging_corner_ = 0;
    return;
  }
  auto rtRect = paint_helper::getCircleRectByPoint(
      capRect.x() + capRect.width() + CORNER_OFFSET,
      capRect.y() - CORNER_OFFSET, effectiveRadius);
  if (rtRect.contains(mousePos)) {
    this->dragging_corner_ = 1;
    return;
  }
  auto lbRect = paint_helper::getCircleRectByPoint(
      capRect.x() - CORNER_OFFSET,
      capRect.y() + capRect.height() + CORNER_OFFSET, effectiveRadius);
  if (lbRect.contains(mousePos)) {
    this->dragging_corner_ = 2;
    return;
  }
  auto rbRect = paint_helper::getCircleRectByPoint(
      capRect.x() + capRect.width() + CORNER_OFFSET,
      capRect.y() + capRect.height() + CORNER_OFFSET, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    this->dragging_corner_ = 3;
    return;
  }
  // 否则也不是角落点击
  this->dragging_corner_ = -1;
}

void CapturedLayer::mouseReleaseEvent() {
  this->resetStatus();
}

void CapturedLayer::resetStatus() {
  this->is_area_dragging_ = false;
  this->dragging_corner_ = -1;
}
