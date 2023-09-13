#include "shape_manager.h"
#include "core/base/point.h"
#include "rect_shape.h"
namespace capi {
ShapeManager::ShapeManager() : selected_shape_(nullptr) {}
void ShapeManager::addShape(ShapeType type, ShapeConfig &config, bool selected) {
  Shape *shape = nullptr;
  switch (type) {
    case ShapeType::Rectangle:shape = new RectShape(config);
    case ShapeType::Ellipse:
    default:break;
  }
  if (shape == nullptr) {
    return;
  }
  if (selected) {
    shape->setIsSelected(true);
    selected_shape_ = shape;
  }
  shapes_.push_back(shape);
}
void ShapeManager::deleteSelectedShape() {
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
void ShapeManager::selectShape(const Point &mousePos) {
  bool hasTouchedContent = false;
  for (auto sp : shapes_) {
    // 清理所有的shape的被选择状态
    sp->setIsSelected(false);
    // 根据优先级原则，若已经选择到了某个图形，我们就不再进行后面图形的判断
    if (hasTouchedContent) {
      continue;
    }
    if (sp->checkPart(mousePos) == ShapePart::Body) {
      sp->setIsSelected(true);
      selected_shape_ = sp;
    }
  }
}
void ShapeManager::hoverShape(const Point &mousePos) {
  bool hasTouchedContent = false;
  for (auto &sp : this->shapes_) {
    sp->setIsHover(false);
    if (hasTouchedContent) {
      continue;
    }
    if (sp->checkPart(mousePos) == ShapePart::Body) {
      sp->setIsHover(true);
    }
  }
}
void ShapeManager::moveSelectedShapeTo(int levelIdx) {
  auto size = shapes_.size();
  if (shapes_.empty()) {
    return;
  }

  // 搜索被选择的图形和其索引
  int selectedIdx = -1;
  Shape *selectedShape = nullptr;
  for (int i = 0; i < size; ++i) {
    auto sp = shapes_[i];
    if (sp->is_selected()) {
      selectedIdx = i;
      selectedShape = sp;
      break;
    }
  }

  if (selectedIdx < 0) {
    return;
  }
  long targetPos;
  if (levelIdx < 0) {
    // 小于0，则视为放到首位
    targetPos = 0;
  } else if (levelIdx >= size) {
    // 超过size，则视为放到最后一个位置
    targetPos = static_cast<long>(size - 1);
  } else {
    // 否则就是对应位置
    targetPos = levelIdx;
  }
  if (targetPos == selectedIdx) {
    // 位置不变，不发生改变
    return;
  }
  // 准备移动
  // 先移除，再添加到指定位置
  shapes_.erase(shapes_.begin() + selectedIdx);
  shapes_.insert(shapes_.begin() + targetPos, selectedShape);
}
void ShapeManager::onMousePress(const Point &mousePos) {

  if (selected_shape_ == nullptr) {
    // 没有选择到任何的图形，就进行一遍查找
    this->selectShape(mousePos);
  }
}
void ShapeManager::onMouseMove(const Point &mousePos) {
  // 记录鼠标上一次位置，并更新当前位置
  auto lastMousePos = mouse_current_pos_;
  mouse_current_pos_ = mousePos;

  if (selected_shape_dragging_part_ == None || selected_shape_ == nullptr) {
    // 不处于拖动态，或没有被选择的图形，不做任何事情
    return;
  }
  // 有拖动场景，先计算拖动距离
  int dx = mouse_current_pos_.x() - lastMousePos.x();
  int dy = mouse_current_pos_.y() - lastMousePos.y();
  // 拖动整个图形，则整体移动
  if (selected_shape_dragging_part_ == Body) {
    selected_shape_->movePosition(dx, dy);
    return;
  }
}
void ShapeManager::onMouseRelease(const Point &mousePos) {
  // 松开鼠标，则关闭拖动状态
  selected_shape_dragging_part_ = None;
}
void ShapeManager::onPaint(Painter *painter) {

}
}

