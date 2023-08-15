
#include <QKeyEvent>
#include <QPainter>
#include <QRect>

#include "../../helper/math_helper.h"
#include "../../helper/paint_helper.h"
#include "captured_layer.h"
#include "operation_mode.h"

const int CORNER_OFFSET = 5;
const int CORNER_CIRCLE_RADIUS = 3;

CapturedLayer::CapturedLayer(QImage *canvasImg, QSize canvasSize)
    : canvas_img_(canvasImg), canvas_size_(canvasSize),
      mouse_last_pos_(QPoint()), opr_mode_(OperationMode::Normal) {}

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

  QSize canvasSize = this->canvas_size_;
  int canvasW = canvasSize.width();
  int canvasH = canvasSize.height();

  QRect leftMaskRect(0, 0, rX, canvasH);
  QRect rightMaskRect(rX + rW, 0, canvasW - (rX + rW), canvasH);
  QRect topMaskRect(rX, 0, rW, rY);
  QRect bottomMaskRect(rX, (rY + rH), rW, canvasH - (rY + rH));

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
  const auto lt = math_helper::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rt = math_helper::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto lb = math_helper::getCircleRectByPoint(
      rX - CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  const auto rb = math_helper::getCircleRectByPoint(
      rX + rW + CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
  painter.setBrush(color);
  painter.drawEllipse(lt);
  painter.drawEllipse(rt);
  painter.drawEllipse(lb);
  painter.drawEllipse(rb);

  painter.restore();
}

void CapturedLayer::mouseDoubleClickEvent(QMouseEvent *) {
  // 双击以后，完成图像保存
  emit signalSaveCapturedRect(this->captured_rect_,
                              CapturedRectSaveType::ToClipboard);
}

void CapturedLayer::mouseMoveEvent(QMouseEvent *event) {

  auto currPos = event->pos();
  auto lastPos = this->mouse_last_pos_;
  // 更新鼠标位置，为下一次move作准备
  this->mouse_last_pos_ = currPos;
  // 计算偏移
  int dx = currPos.x() - lastPos.x();
  int dy = currPos.y() - lastPos.y();

  if (this->opr_mode_ == OperationMode::Normal) {
    return;
  }

  QRect lastCapRect = this->captured_rect_;
  if (this->opr_mode_ == OperationMode::DraggingArea) {
    // 移动捕获区域
    this->captured_rect_ = QRect(lastCapRect.x() + dx, lastCapRect.y() + dy,
                                 lastCapRect.width(), lastCapRect.height());
  } else if (this->opr_mode_ == DraggingLeftTop ||
             this->opr_mode_ == DraggingRightTop ||
             this->opr_mode_ == DraggingLeftBottom ||
             this->opr_mode_ == DraggingRightBottom) {
    // 4个角拖动
    auto lt = QPoint(lastCapRect.x(), lastCapRect.y());
    auto rt = QPoint(lt.x() + lastCapRect.width(), lt.y());
    auto lb = QPoint(lt.x(), lt.y() + lastCapRect.height());
    auto rb = QPoint(rt.x(), lb.y());
    QPoint *draggingCor; // 正在拖动的角
    QPoint *fixedCor;    // 正在拖动角的对角固定不动
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

  if (this->opr_mode_ == EnableDrawRect) {
    // 用户启用了矩形绘制，则进入矩形绘制状态
    this->opr_mode_ = DrawingRect;
    return;
  }

  if (math_helper::posInEffectiveRect(mousePos, this->captured_rect_, 10)) {
    // 在有效区域内点击，则拖动rect
    this->opr_mode_ = DraggingArea;
    return;
  }
  // 否则，不属于区域拖动
  // 开始判断是否点击到了某个角
  auto effectiveRadius = CORNER_CIRCLE_RADIUS * 2;
  auto capRect = this->captured_rect_;
  auto ltRect = math_helper::getCircleRectByPoint(capRect.x() - CORNER_OFFSET,
                                                  capRect.y() - CORNER_OFFSET,
                                                  effectiveRadius);
  if (ltRect.contains(mousePos)) {
    this->opr_mode_ = DraggingLeftTop;
    return;
  }
  auto rtRect = math_helper::getCircleRectByPoint(
      capRect.x() + capRect.width() + CORNER_OFFSET,
      capRect.y() - CORNER_OFFSET, effectiveRadius);
  if (rtRect.contains(mousePos)) {
    this->opr_mode_ = DraggingRightTop;
    return;
  }
  auto lbRect = math_helper::getCircleRectByPoint(
      capRect.x() - CORNER_OFFSET,
      capRect.y() + capRect.height() + CORNER_OFFSET, effectiveRadius);
  if (lbRect.contains(mousePos)) {
    this->opr_mode_ = DraggingLeftBottom;
    return;
  }
  auto rbRect = math_helper::getCircleRectByPoint(
      capRect.x() + capRect.width() + CORNER_OFFSET,
      capRect.y() + capRect.height() + CORNER_OFFSET, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    this->opr_mode_ = DraggingRightBottom;
    return;
  }
  // 否则也不是角落点击
  this->opr_mode_ = Normal;
}

void CapturedLayer::mouseReleaseEvent() { 
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

void CapturedLayer::keyReleaseEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_Escape) {
    emit signalQuitCurrentLayer();
  }
}

void CapturedLayer::resetStatus() {
  this->opr_mode_ = Normal;
}
void CapturedLayer::setCanvasSize(QSize size) { this->canvas_size_ = size; }
