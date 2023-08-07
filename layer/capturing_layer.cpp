#include "capturing_layer.h"
//#include "screenshot_utils.h"
#include "helper/math_helper.h"
#include "helper/paint_helper.h"

CapturingLayer::CapturingLayer(QImage *screenPic) : screen_pic_(screenPic) {}

void CapturingLayer::paint(QPainter &painter) {
  painter.save();
  painter.setPen(QPen(QColor(0, 111, 222), 1));
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  int dX = this->start_pos_.x();
  int dY = this->start_pos_.y();
  QRect rect = math_helper::calcRect(mX, mY, dX, dY);
  painter.drawRect(rect);

  paint_helper::paintCapturingRectSizeTip(painter, this->mouse_pos_,
                                          this->start_pos_, rect.size());
  painter.restore();
}

void CapturingLayer::mouseMoveEvent(QMouseEvent *event) {
  this->mouse_pos_ = event->pos();
}

void CapturingLayer::setStartPos(QPoint pos) {
  this->start_pos_ = pos;
}

void CapturingLayer::mouseReleaseEvent(QMouseEvent *event) {
  QPoint &startPos = this->start_pos_;
  QPoint currentPos = event->pos();
  QRect capturedRect = math_helper::calcRect(currentPos.x(), currentPos.y(),
                                             startPos.x(), startPos.y());

  if (!math_helper::sizeLengthOver(capturedRect.size(), 10)) {
    // 截图区域过于小，不予捕获，回到Explore状态
    emit this->capturingFinishedSignal(false, nullptr);
  } else {
    emit this->capturingFinishedSignal(true, &capturedRect);
  }
}

CapturingLayer::~CapturingLayer() {}
