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

ShapePart Shape::CheckPart(const Point &mouse_pos) const {
  // 首先检查是否点击到了body
  if (math_utils::CheckPosInEffectiveRect(mouse_pos, this->content_rect(), 10)) {
    return Body;
  }
  auto effective_radius = anchor_size_ * 2;
  // 线图形场景，起始、终点检查
  if (this->is_line_shape()) {
    // 线图形锚点不做偏移处理，直接就是两个端点锚点
    auto start_anchor_rect = math_utils::GetSquareByPoint(start_pos_.x(),
                                                          start_pos_.y(),
                                                          effective_radius);
    if (start_anchor_rect.Contains(mouse_pos)) {
      return LineStart;
    }
    auto end_anchor_rect = math_utils::GetSquareByPoint(end_pos_.x(),
                                                        end_pos_.y(),
                                                        effective_radius);
    if (end_anchor_rect.Contains(mouse_pos)) {
      return LineEnd;
    }

    return None;
  }
  // 非线图形的四个角检查
  auto content_rect = this->content_rect();
  auto lt_rect = math_utils::GetSquareByPoint(content_rect.x() - anchor_offset_,
                                              content_rect.y() - anchor_offset_,
                                              effective_radius);
  if (lt_rect.Contains(mouse_pos)) {
    return LeftTop;
  }
  auto rt_rect = math_utils::GetSquareByPoint(
      content_rect.x() + content_rect.w() + anchor_offset_, content_rect.y() - anchor_offset_,
      effective_radius);
  if (rt_rect.Contains(mouse_pos)) {
    return RightTop;
  }
  auto lb_rect = math_utils::GetSquareByPoint(
      content_rect.x() - anchor_offset_, content_rect.y() + content_rect.h() + anchor_offset_,
      effective_radius);
  if (lb_rect.Contains(mouse_pos)) {
    return LeftBottom;
  }
  auto rb_rect = math_utils::GetSquareByPoint(
      content_rect.x() + content_rect.w() + anchor_offset_,
      content_rect.y() + content_rect.h() + anchor_offset_, effective_radius);
  if (rb_rect.Contains(mouse_pos)) {
    return RightBottom;
  }
  return None;
}

void Shape::OnPaint(Painter *painter) {
  OnContentPaint(painter);
  // 由于后绘制的覆盖先绘制的，选中状态下，选中渲染应该“覆盖”在原有图像上
  if (this->is_selected_) {
    OnSelectedStatusAnchorPaint(painter, nullptr);
  }
}

void Shape::OnSelectedStatusAnchorPaint(Painter *painter,
                                        std::function<void(Painter *, const Rect &)> anchor_shape_paint_cb) {
  // 默认根据当前是否被hover和是否被选中来进行绘制整个图形的边框
  if (!this->is_selected_) {
    return;
  }
  painter->Save();
  std::vector<Rect> anchors;
  if (is_line_shape()) {
    auto startAnchor = math_utils::GetSquareByPoint(start_pos_.x(), start_pos_.y(), anchor_size_);
    auto endAnchor = math_utils::GetSquareByPoint(end_pos_.x(), end_pos_.y(), anchor_size_);
    anchors.push_back(startAnchor);
    anchors.push_back(endAnchor);
  } else {
    auto content_rect = this->content_rect();
    // 非 “线” 图形，绘制四个角，一定的偏移，效果好点
    int rX = content_rect.x();
    int rY = content_rect.y();
    int rW = content_rect.w();
    int rH = content_rect.h();
    auto lt = math_utils::GetSquareByPoint(
        rX - anchor_offset_, rY - anchor_offset_, anchor_size_);
    auto rt = math_utils::GetSquareByPoint(
        rX + rW + anchor_offset_, rY - anchor_offset_, anchor_size_);
    auto lb = math_utils::GetSquareByPoint(
        rX - anchor_offset_, rY + rH + anchor_offset_, anchor_size_);
    auto rb = math_utils::GetSquareByPoint(
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
      painter->SetBrush(Brush(Color(0, 111, 222)));
      painter->DrawRect(anchor_rect);
      // 在绘制正方形边框
      painter->SetPen(Pen(Color(255, 255, 255)));
      painter->DrawRect(anchor_rect);
    } else {
      anchor_shape_paint_cb(painter, anchor_rect);
    }
  }
  painter->Restore();
}

bool Shape::is_selected() const {
  return is_selected_;
}

void Shape::SetIsHover(bool isHover) {
  is_hover_ = isHover;
}

void Shape::setIsSelected(bool isSelected) {
  is_selected_ = isSelected;
}

const Point &Shape::start_pos() const {
  return start_pos_;
}
void Shape::SetStartPos(const Point &start_pos) {
  start_pos_ = start_pos;
}
const Point &Shape::end_pos() const {
  return end_pos_;
}
void Shape::SetEndPos(const Point &end_pos) {
  end_pos_ = end_pos;
}
Rect Shape::content_rect() const {
  auto contentRect = math_utils::CalcRect(start_pos_.x(), start_pos_.y(), end_pos_.x(), end_pos_.y());
  return contentRect;
}
void Shape::MovePosition(int dx, int dy) {
  auto sp = this->start_pos_;
  auto ep = this->end_pos_;
  this->start_pos_.SetX(sp.x() + dx);
  this->start_pos_.SetY(sp.y() + dy);
  this->end_pos_.SetX(ep.x() + dx);
  this->end_pos_.SetY(ep.y() + dy);
}
bool Shape::is_line_shape() const {
  return false;
}
Shape::~Shape() = default;

};