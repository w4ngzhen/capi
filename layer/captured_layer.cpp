#include "captured_layer.h"
#include "helper/math_helper.h"
#include "helper/paint_helper.h"

#include <QPainter>
#include <QRect>

CapturedLayer::CapturedLayer(QImage *screenPic, QSize screenSize,
                             QSize screenScale)
    : screen_pic_(screenPic), screen_size_(screenSize),
      screen_scale_(screenScale), is_relocating_(false),
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
  const int radius = 3;
  const auto c1 = paint_helper::getCircleRectByPoint(rX, rY, radius);
  const auto c2 = paint_helper::getCircleRectByPoint(rX + rW, rY, radius);
  const auto c3 = paint_helper::getCircleRectByPoint(rX, rY + rH, radius);
  const auto c4 = paint_helper::getCircleRectByPoint(rX + rW, rY + rH, radius);
  painter.setBrush(color);
  painter.drawEllipse(c1);
  painter.drawEllipse(c2);
  painter.drawEllipse(c3);
  painter.drawEllipse(c4);

  painter.restore();
}

void CapturedLayer::mouseDoubleClickEvent(QMouseEvent *) {
  auto logicRect = this->captured_rect_;
  auto realRect = paint_helper::rectLogicPixelToRealPixel(logicRect, 2, 2);
  emit saveCapturedRectSignal(logicRect, realRect,
                              CapturedRectSaveType::ToClipboard);
}

void CapturedLayer::mouseMoveEvent(QMouseEvent *event) {

  auto currPos = event->pos();

  if (!this->is_relocating_) {
    return;
  }

  auto lastPos = this->mouse_last_pos_;
  int dx = currPos.x() - lastPos.x();
  int dy = currPos.y() - lastPos.y();

  // 更新捕获区域
  QRect lastCapRect = this->captured_rect_;
  this->captured_rect_ = QRect(lastCapRect.x() + dx, lastCapRect.y() + dy,
                               lastCapRect.width(), lastCapRect.height());

  // 更新鼠标位置，为下一次move作准备
  this->mouse_last_pos_ = currPos;

  // 外部需要update
}

void CapturedLayer::mousePressEvent(QMouseEvent *event) {
  auto mousePos = event->pos();

  if (math_helper::posInEffectiveRect(mousePos, this->captured_rect_, 10)) {
    // 在有效区域内点击，则拖动rect
    this->is_relocating_ = true;
    this->mouse_last_pos_.setX(mousePos.x());
    this->mouse_last_pos_.setY(mousePos.y());
    return;
  }
}

void CapturedLayer::mouseReleaseEvent() { this->is_relocating_ = false; }

void CapturedLayer::resetStatus() {
  this->is_relocating_ = false;
}
