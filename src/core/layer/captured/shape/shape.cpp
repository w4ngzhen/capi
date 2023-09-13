#include "core/paint/painter.h"
#include "core/base/point.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"
#include "shape.h"

const int CORNER_OFFSET = 5;
const int CORNER_CIRCLE_RADIUS = 3;

namespace capi {

Shape::Shape(const ShapeConfig &config) : config_(config) {
}

ShapePart Shape::checkPart(const Point &mousePos) const {
  // 首先检查是否点击到了body
  if (math_utils::posInEffectiveRect(mousePos, this->content_rect(), 10)) {
    return Body;
  }
  auto effectiveRadius = CORNER_CIRCLE_RADIUS * 2;
  // 线图形场景，起始、终点检查
  if (this->is_line_shape()) {
    auto startRect = math_utils::getCircleRectByPoint(startPos_.x() - CORNER_OFFSET,
                                                      startPos_.y() - CORNER_OFFSET,
                                                      effectiveRadius);
    if (startRect.contains(mousePos)) {
      return LineStart;
    }
    auto endRect = math_utils::getCircleRectByPoint(endPos_.x() - CORNER_OFFSET,
                                                    endPos_.y() - CORNER_OFFSET,
                                                    effectiveRadius);
    if (endRect.contains(mousePos)) {
      return LineEnd;
    }

    return None;
  }
  // 非线图形的四个角检查
  auto capRect = this->content_rect();
  auto ltRect = math_utils::getCircleRectByPoint(capRect.x() - CORNER_OFFSET,
                                                 capRect.y() - CORNER_OFFSET,
                                                 effectiveRadius);
  if (ltRect.contains(mousePos)) {
    return LeftTop;
  }
  auto rtRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET, capRect.y() - CORNER_OFFSET,
      effectiveRadius);
  if (rtRect.contains(mousePos)) {
    return RightTop;
  }
  auto lbRect = math_utils::getCircleRectByPoint(
      capRect.x() - CORNER_OFFSET, capRect.y() + capRect.h() + CORNER_OFFSET,
      effectiveRadius);
  if (lbRect.contains(mousePos)) {
    return LeftBottom;
  }
  auto rbRect = math_utils::getCircleRectByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET,
      capRect.y() + capRect.h() + CORNER_OFFSET, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    return RightBottom;
  }
  return None;
}

void Shape::onPaint(Painter *painter) {
  onBorderPaint(painter);
  onContentPaint(painter);
}

void Shape::onBorderPaint(Painter *painter) {
  // 默认根据当前是否被hover和是否被选中来进行绘制整个图形的边框
  if (!this->is_hover_ && !this->is_selected_) {
    return;
  }
  painter->save();
  if (this->is_selected_) {
    // 原本content矩形
    auto contentR = math_utils::enlargeRect(this->content_rect(), 5);
    // 我们绘制边框的时候需要比实际大一点，效果会更好

    // 绘制边框
    Pen pen(1, Color(200, 200, 200), PenStyle::DashLine);
    painter->setPen(pen);
    painter->drawRect(contentR);
    // 如果是非线图形，支持resize，此时需要绘制四个角，允许用户resize图形
    if (!is_line_shape()) {
      int rX = contentR.x();
      int rY = contentR.y();
      int rW = contentR.w();
      int rH = contentR.h();
      const auto lt = math_utils::getCircleRectByPoint(
          rX - CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
      const auto rt = math_utils::getCircleRectByPoint(
          rX + rW + CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
      const auto lb = math_utils::getCircleRectByPoint(
          rX - CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
      const auto rb = math_utils::getCircleRectByPoint(
          rX + rW + CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_CIRCLE_RADIUS);
      // 先填充圆
      painter->setBrush(Brush(Color(0, 111, 222)));
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
    }
  }
  painter->restore();
}

bool Shape::is_selected() const {
  return is_selected_;
}

void Shape::setIsHover(bool isHover) {
  is_hover_ = isHover;
}

void Shape::setIsSelected(bool isSelected) {
  is_selected_ = isSelected;
}

const Point &Shape::start_pos() const {
  return startPos_;
}
void Shape::setStartPos(const Point &start_pos) {
  startPos_ = start_pos;
}
const Point &Shape::end_pos() const {
  return endPos_;
}
void Shape::setEndPos(const Point &end_pos) {
  endPos_ = end_pos;
}
Rect Shape::content_rect() const {
  auto contentRect = math_utils::calcRect(startPos_.x(), startPos_.y(), endPos_.x(), endPos_.y());
  return contentRect;
}
void Shape::movePosition(int dx, int dy) {
  auto sp = this->startPos_;
  auto ep = this->endPos_;
  this->startPos_.setX(sp.x() + dx);
  this->startPos_.setY(sp.y() + dy);
  this->endPos_.setX(ep.x() + dx);
  this->endPos_.setY(ep.y() + dy);
}
const bool Shape::is_line_shape() const {
  return false;
}

};