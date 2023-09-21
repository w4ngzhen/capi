#include "core/paint/painter.h"
#include "core/base/point.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"
#include "shape.h"
#include <vector>

namespace capi {

Shape::Shape(const ShapeConfig &config) : config_(config) {
}

Shape::Shape() : config_(ShapeConfig()) {}

ShapePart Shape::checkPart(const Point &mousePos) const {
  // 首先检查是否点击到了body
  if (math_utils::posInEffectiveRect(mousePos, this->content_rect(), 10)) {
    return Body;
  }
  auto effectiveRadius = anchor_size_ * 2;
  // 线图形场景，起始、终点检查
  if (this->is_line_shape()) {
    // 线图形锚点不做偏移处理，直接就是两个端点锚点
    auto start_anchor_rect = math_utils::getSquareByPoint(startPos_.x(),
                                                          startPos_.y(),
                                                          effectiveRadius);
    if (start_anchor_rect.contains(mousePos)) {
      return LineStart;
    }
    auto end_anchor_rect = math_utils::getSquareByPoint(endPos_.x(),
                                                        endPos_.y(),
                                                        effectiveRadius);
    if (end_anchor_rect.contains(mousePos)) {
      return LineEnd;
    }

    return None;
  }
  // 非线图形的四个角检查
  auto capRect = this->content_rect();
  auto ltRect = math_utils::getSquareByPoint(capRect.x() - anchor_offset_,
                                             capRect.y() - anchor_offset_,
                                             effectiveRadius);
  if (ltRect.contains(mousePos)) {
    return LeftTop;
  }
  auto rtRect = math_utils::getSquareByPoint(
      capRect.x() + capRect.w() + anchor_offset_, capRect.y() - anchor_offset_,
      effectiveRadius);
  if (rtRect.contains(mousePos)) {
    return RightTop;
  }
  auto lbRect = math_utils::getSquareByPoint(
      capRect.x() - anchor_offset_, capRect.y() + capRect.h() + anchor_offset_,
      effectiveRadius);
  if (lbRect.contains(mousePos)) {
    return LeftBottom;
  }
  auto rbRect = math_utils::getSquareByPoint(
      capRect.x() + capRect.w() + anchor_offset_,
      capRect.y() + capRect.h() + anchor_offset_, effectiveRadius);
  if (rbRect.contains(mousePos)) {
    return RightBottom;
  }
  return None;
}

void Shape::onPaint(Painter *painter) {
  onContentPaint(painter);
  // 由于后绘制的覆盖先绘制的，选中状态下，选中渲染应该“覆盖”在原有图像上
  if (this->is_selected_) {
    onSelectedStatusAnchorPaint(painter, nullptr);
  }
}

void Shape::onSelectedStatusAnchorPaint(Painter *painter,
                                        const std::function<void(Painter *, const Rect &)> anchor_shape_paint_cb) {
  // 默认根据当前是否被hover和是否被选中来进行绘制整个图形的边框
  if (!this->is_selected_) {
    return;
  }
  painter->save();
  std::vector<Rect> anchors;
  if (is_line_shape()) {
    auto startAnchor = math_utils::getSquareByPoint(startPos_.x(), startPos_.y(), anchor_size_);
    auto endAnchor = math_utils::getSquareByPoint(endPos_.x(), endPos_.y(), anchor_size_);
    anchors.push_back(startAnchor);
    anchors.push_back(endAnchor);
  } else {
    auto contentR = this->content_rect();
    // 非 “线” 图形，绘制四个角，一定的偏移，效果好点
    int rX = contentR.x();
    int rY = contentR.y();
    int rW = contentR.w();
    int rH = contentR.h();
    auto lt = math_utils::getSquareByPoint(
        rX - anchor_offset_, rY - anchor_offset_, anchor_size_);
    auto rt = math_utils::getSquareByPoint(
        rX + rW + anchor_offset_, rY - anchor_offset_, anchor_size_);
    auto lb = math_utils::getSquareByPoint(
        rX - anchor_offset_, rY + rH + anchor_offset_, anchor_size_);
    auto rb = math_utils::getSquareByPoint(
        rX + rW + anchor_offset_, rY + rH + anchor_offset_, anchor_size_);
    anchors.push_back(lt);
    anchors.push_back(rt);
    anchors.push_back(lb);
    anchors.push_back(rb);
  }
  // 得到待绘制的锚点列表后，进行绘制操作
  for (const auto &anchor_rect : anchors) {
    if (anchor_shape_paint_cb == nullptr) {
      // 如果没有提供锚点图形绘制回调，则使用默认绘制方式
      // 先填充正方形
      painter->setBrush(Brush(Color(0, 111, 222)));
      painter->drawRect(anchor_rect);
      // 在绘制正方形边框
      painter->setPen(Pen(Color(255, 255, 255)));
      painter->drawRect(anchor_rect);
    } else {
      anchor_shape_paint_cb(painter, anchor_rect);
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