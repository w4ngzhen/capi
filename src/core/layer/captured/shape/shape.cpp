#include "core/paint/painter.h"
#include "core/base/point.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"
#include "shape.h"
#include <vector>

const int CORNER_OFFSET = 5;
const int CORNER_SQUARE_SIZE = 7;

namespace capi {

Shape::Shape(const ShapeConfig &config) : config_(config) {
}

Shape::Shape() : config_(ShapeConfig()) {}

ShapePart Shape::checkPart(const Point &mousePos) const {
  // 首先检查是否点击到了body
  if (math_utils::posInEffectiveRect(mousePos, this->content_rect(), 10)) {
    return Body;
  }
  auto effectiveRadius = CORNER_SQUARE_SIZE * 2;
  // 线图形场景，起始、终点检查
  if (this->is_line_shape()) {
    auto startRect = math_utils::getSquareByPoint(startPos_.x() - CORNER_OFFSET,
                                                  startPos_.y() - CORNER_OFFSET,
                                                  effectiveRadius);
    if (startRect.contains(mousePos)) {
      return LineStart;
    }
    auto endRect = math_utils::getSquareByPoint(endPos_.x() - CORNER_OFFSET,
                                                endPos_.y() - CORNER_OFFSET,
                                                effectiveRadius);
    if (endRect.contains(mousePos)) {
      return LineEnd;
    }

    return None;
  }
  // 非线图形的四个角检查
  auto capRect = this->content_rect();
  auto ltRect = math_utils::getSquareByPoint(capRect.x() - CORNER_OFFSET,
                                             capRect.y() - CORNER_OFFSET,
                                             effectiveRadius);
  if (ltRect.contains(mousePos)) {
    return LeftTop;
  }
  auto rtRect = math_utils::getSquareByPoint(
      capRect.x() + capRect.w() + CORNER_OFFSET, capRect.y() - CORNER_OFFSET,
      effectiveRadius);
  if (rtRect.contains(mousePos)) {
    return RightTop;
  }
  auto lbRect = math_utils::getSquareByPoint(
      capRect.x() - CORNER_OFFSET, capRect.y() + capRect.h() + CORNER_OFFSET,
      effectiveRadius);
  if (lbRect.contains(mousePos)) {
    return LeftBottom;
  }
  auto rbRect = math_utils::getSquareByPoint(
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
    std::vector<Rect> corners;
    if (is_line_shape()) {
      auto startAnchor = math_utils::getSquareByPoint(startPos_.x(), startPos_.y(), CORNER_SQUARE_SIZE);
      auto endAnchor = math_utils::getSquareByPoint(endPos_.x(), endPos_.y(), CORNER_SQUARE_SIZE);
      corners.push_back(startAnchor);
      corners.push_back(endAnchor);
    } else {
      auto contentR = this->content_rect();
      // 非 “线” 图形，绘制四个角，一定的偏移，效果好点
      int rX = contentR.x();
      int rY = contentR.y();
      int rW = contentR.w();
      int rH = contentR.h();
      auto lt = math_utils::getSquareByPoint(
          rX - CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_SQUARE_SIZE);
      auto rt = math_utils::getSquareByPoint(
          rX + rW + CORNER_OFFSET, rY - CORNER_OFFSET, CORNER_SQUARE_SIZE);
      auto lb = math_utils::getSquareByPoint(
          rX - CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_SQUARE_SIZE);
      auto rb = math_utils::getSquareByPoint(
          rX + rW + CORNER_OFFSET, rY + rH + CORNER_OFFSET, CORNER_SQUARE_SIZE);
      corners.push_back(lt);
      corners.push_back(rt);
      corners.push_back(lb);
      corners.push_back(rb);
    }
    // 得到待绘制的锚点列表后，进行绘制操作
    for (const auto &item : corners) {
      // 先填充正方形
      painter->setBrush(Brush(Color(0, 111, 222)));
      painter->drawRect(item);
      // 在绘制正方形边框
      painter->setPen(Pen(Color(255, 255, 255)));
      painter->drawRect(item);
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
bool Shape::is_line_shape() const {
  return false;
}
Shape::~Shape() = default;

};