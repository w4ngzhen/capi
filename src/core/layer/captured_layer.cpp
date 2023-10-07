#include "captured_layer.h"
#include "core/base/point.h"
#include "core/utils/math_utils.h"
#include "core/layer/shape/captured_shape.h"
#include "core/layer/shape/rect_shape.h"
#include "core/layer/shape/ellipse_shape.h"
#include "core/layer/shape/line_shape.h"
namespace capi {
CapturedLayer::CapturedLayer(const Size &canvas_size) : Layer(canvas_size) {
  selected_shape_dragging_part_ = None;
  mouse_press_pos_ = Point();
  // 根据被捕获的矩形信息，构造一个CapturedShape，默认选中
  auto cap_shape = new CapturedShape();
  shapes_.push_back(cap_shape);
}

void CapturedLayer::SetCapturedRect(const Rect &captured_rect) {
  auto cap_shape = GetCapturedShape();
  if (cap_shape != nullptr) {
    auto start_pos = captured_rect.pos();
    auto end_pos = Point(captured_rect.x() + captured_rect.w(),
                         captured_rect.y() + captured_rect.h());
    cap_shape->SetStartPos(start_pos);
    cap_shape->SetEndPos(end_pos);
    // 并默认选中
    cap_shape->SetIsSelected(true);
    selected_shape_ = cap_shape;
  }
}

void CapturedLayer::AddShape(ShapeType type, const ShapeConfig &config, bool selected) {
  Shape *shape = nullptr;
  switch (type) {
    case ShapeType::Rectangle:shape = new RectShape(config);
      break;
    case ShapeType::Ellipse: shape = new EllipseShape(config);
      break;
    case ShapeType::Line: shape = new LineShape(config);
    default:break;
  }
  if (shape == nullptr) {
    return;
  }
  if (selected) {
    shape->SetIsSelected(true);
    selected_shape_ = shape;
  }
  shapes_.push_back(shape);
}
void CapturedLayer::DeleteSelectedShape() {
  if (selected_shape_ == nullptr) {
    return;
  }
  if (selected_shape_->type() == 0xFFFF) {
    // 如果被选择的图形是核心捕获图形，不能进行删除操作
    return;
  }
  auto itr = std::find_if(
      shapes_.begin(),
      shapes_.end(),
      [](Shape *sp) { return sp->is_selected(); }
  );
  if (itr != shapes_.end()) {
    shapes_.erase(itr);
    selected_shape_ = nullptr;
  }
}
void CapturedLayer::SelectShape(const Point &mouse_pos) {
  bool has_touched_content = false;
  for (auto sp : shapes_) {
    // 清理所有的shape的被选择状态
    sp->SetIsSelected(false);
    // 根据优先级原则，若已经选择到了某个图形，我们就不再进行后面图形的判断
    if (has_touched_content) {
      continue;
    }
    if (sp->CheckPart(mouse_pos) == ShapePart::Body) {
      sp->SetIsSelected(true);
      selected_shape_ = sp;
    }
  }
}
void CapturedLayer::HoverShape(const Point &mouse_pos) {
  Shape *last_hover_sp = nullptr;
  for (auto &sp : this->shapes_) {
    if (sp->CheckPart(mouse_pos) == Body) {
      if (last_hover_sp != nullptr) {
        last_hover_sp->SetIsHover(false);
      }
      sp->SetIsHover(true);
      last_hover_sp = sp;
    }
  }
}
void CapturedLayer::MoveSelectedShapeToTargetLevel(int level_idx) {

  if (selected_shape_ == nullptr) {
    return;
  }

  /**
   * CapturedShape 始终处于图层索引为0的位置
   * 无法移动 CapturedShape，以及将任何其他的图形移动到 CapturedShape 的下层
   */
  if (level_idx == 0 || selected_shape_->type() == 0xFFFF) {
    return;
  }

  auto size = shapes_.size();
  if (shapes_.empty()) {
    return;
  }

  // 搜索被选择的图形和其索引
  int selected_idx = -1;
  Shape *selected_shape = nullptr;
  for (int i = 0; i < size; ++i) {
    auto sp = shapes_[i];
    if (sp->is_selected()) {
      selected_idx = i;
      selected_shape = sp;
      break;
    }
  }

  if (selected_idx < 0) {
    return;
  }
  long target_pos;
  if (level_idx < 0) {
    // 小于0，则视为放到首位
    target_pos = 0;
  } else if (level_idx >= size) {
    // 超过size，则视为放到最后一个位置
    target_pos = static_cast<long>(size - 1);
  } else {
    // 否则就是对应位置
    target_pos = level_idx;
  }
  if (target_pos == selected_idx) {
    // 位置不变，不发生改变
    return;
  }
  // 准备移动
  // 先移除，再添加到指定位置
  shapes_.erase(shapes_.begin() + selected_idx);
  shapes_.insert(shapes_.begin() + target_pos, selected_shape);
}
void CapturedLayer::OnMousePress(const Point &mouse_pos) {
  Shape *temp_selected_sp = nullptr;
  for (auto &sp : this->shapes_) {
    auto part = sp->CheckPart(mouse_pos);
    // 如果点击到了某个区域，我们就认为“碰”到了该图形
    if (part != None) {
      selected_shape_dragging_part_ = part;
      /**
       * 点击到了某个图形某个区域，则该图形此刻要被选中，
       * 但在选中前，需要 先判断是不是已经有了被选中的图形
       * 如有则先把前面被视为选中的图形置为false
       */
      if (temp_selected_sp != nullptr) {
        temp_selected_sp->SetIsSelected(false);
      }
      sp->SetIsSelected(true);
      temp_selected_sp = sp;
    } else {
      sp->SetIsSelected(false);
    }
  }
  if (temp_selected_sp == nullptr) {
    // 没有点击到任何的图形，则清理成员变量 selected_shape_ 的状态，并置为nullptr
    if (selected_shape_ != nullptr) {
      selected_shape_->SetIsSelected(false);
      selected_shape_ = nullptr;
    }
  } else {
    // 有点击到某个图形，那么这里不需要对 selected_shape_ 状态清理
    // 因为上面遍历的流程已经做了，这里只需要赋值即可
    selected_shape_ = temp_selected_sp;
  }
}
void CapturedLayer::OnMouseMove(const Point &mouse_pos) {

  // 保存鼠标上一次位置，并记录更新当前位置
  auto last_mouse_pos = mouse_current_pos_;
  mouse_current_pos_ = mouse_pos;

  /**
   * 情况：不处于拖动态，或没有被选择的图形
   * 不进行任何的拖动操作，只需要更新hover效果
   */
  if (selected_shape_dragging_part_ == None || selected_shape_ == nullptr) {
    HoverShape(mouse_pos);
    return;
  }

  /**
   * 情况：且有被选择的图形，且正被拖动
   */
  // 先计算拖动距离
  int dx = mouse_current_pos_.x() - last_mouse_pos.x();
  int dy = mouse_current_pos_.y() - last_mouse_pos.y();
  // 拖动整个图形，则整体移动
  if (selected_shape_dragging_part_ == Body) {
    selected_shape_->MovePosition(dx, dy);
    return;
  }
  // 拖动四个角 或 “线”图形拖动开始、结束节点，就要处理resize
  auto &sp = selected_shape_->start_pos();
  auto &ep = selected_shape_->end_pos();
  auto is_line_shape = selected_shape_->is_line_shape();

  //“线”图形，只能拖动开始、结束端点
  if (is_line_shape) {
    if (selected_shape_dragging_part_ == LineStart) {
      selected_shape_->SetStartPos(Point(sp.x() + dx, sp.y() + dy));
      return;
    } else if (selected_shape_dragging_part_ == LineEnd) {
      selected_shape_->SetEndPos(Point(ep.x() + dx, ep.y() + dy));
      return;
    }
    return;
  }
  // 处理非“线”图形，譬如矩形、圆等，它们以矩形作为承载，四个角都能移动
  auto curr_content_rect = selected_shape_->content_rect();
  if (math_utils::CheckIsCornerPart(selected_shape_dragging_part_)) {
    // 使用工具方法计算四个角的拖动
    Rect target_rect{};
    auto is_calc_ok = math_utils::CalcCornerDragRect(
        curr_content_rect,
        dx, dy,
        selected_shape_dragging_part_,
        &target_rect);
    if (!is_calc_ok) {
      return;
    }
    auto new_start_pos = Point(target_rect.x(), target_rect.y());
    auto new_end_pos = Point(new_start_pos.x() + target_rect.w(), new_start_pos.y() + target_rect.h());
    selected_shape_->SetStartPos(new_start_pos);
    selected_shape_->SetEndPos(new_end_pos);
  }

}
void CapturedLayer::OnMouseRelease(const Point &mouse_pos) {
  // 松开鼠标，则关闭拖动状态
  selected_shape_dragging_part_ = None;
}
void CapturedLayer::OnMouseDoubleClick(const Point &) {
  // 准备构造captured canvas_image 保存事件
  // 暂时只支持保存到粘贴板
  // 从集合中找到CapturedShape，获取该Shape的矩形数据
  auto cap_shape = GetCapturedShape();
  if (cap_shape == nullptr) {
    return;
  }
  CapturedImageSaveEvent ev(cap_shape->content_rect(), SaveMode::Clipboard);
  this->layer_event_on_captured_layer_image_save_cb_(&ev);
}

void CapturedLayer::OnPaint(Painter *painter) {
  /**
   * 绘制图形有一个特殊点，由于 CapturedShape 会有蒙版效果，
   * 我们总是最后绘制这个 CapturedShape
   */
  auto count = shapes_.size();
  if (count <= 0) {
    return;
  }
  for (int i = 1; i < count; i++) {
    shapes_[i]->OnPaint(painter);
  }
  auto captured_sp = shapes_[0];
  if (captured_sp != nullptr) {
    captured_sp->OnPaint(painter);
  }
}

void CapturedLayer::OnKeyPress(Key k, KeyboardModifier m) {
  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->layer_event_on_quit_current_layer_cb_();
    return;
  }
  if (k == Key::Key_Delete) {
    // 删除操作
    DeleteSelectedShape();
    return;
  }
  switch (k) {
    case Key::Key_1:AddShape(Rectangle, ShapeConfig(), true);
      break;
    case Key::Key_2:AddShape(Ellipse, ShapeConfig(), true);
      break;
    case Key::Key_3:AddShape(Line, ShapeConfig(), true);
      break;
    default:break;
  }
}

void CapturedLayer::SetLayerEventOnCapturedLayerImageSaveCb(LayerEventOnCapturedLayerImageSaveCb cb) {
  this->layer_event_on_captured_layer_image_save_cb_ = std::move(cb);
}
void CapturedLayer::OnCanvasResize(const Size &size) {
  Layer::OnCanvasResize(size);
  auto cap_shape = GetCapturedShape();
  if (cap_shape != nullptr) {
    dynamic_cast<CapturedShape *>(cap_shape)->SetCanvasSize(size);
  }
}
Shape *CapturedLayer::GetCapturedShape() {
  auto cap_shape_itr = std::find_if(
      shapes_.begin(),
      shapes_.end(),
      [](Shape *sp) {
        return sp->type() == 0xFFFF;
      }
  );
  if (cap_shape_itr != shapes_.end()) {
    return *cap_shape_itr;
  }
  return nullptr;
}

}

