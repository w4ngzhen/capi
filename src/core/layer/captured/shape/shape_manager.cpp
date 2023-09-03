#include "shape_manager.h"
#include "core/base/point.h"
namespace capi {
capi::ShapeManager::ShapeManager() : selected_shape_(nullptr) {

}
void ShapeManager::addShape(bool selected) {

}
void ShapeManager::removeSelectedShape() {
  int idx = findShapeIndex(this->shapes_, [](Shape *item) {
    return item->is_selected();
  });
  if (idx >= 0) {
    this->shapes_.erase(this->shapes_.begin() + idx);
    this->selected_shape_ = nullptr;
  }
}
void ShapeManager::selectShape(const Point &mousePos) {
  Shape *shape = findShape(this->shapes_, [mousePos](Shape *sp) {
    sp->setIsSelected(false);
    return sp->checkTouchedArea(mousePos) == TouchedArea::Body;
  });
  // 存在被选中的图形，则设置对应的被选中状态
  shape->setIsSelected(true);
  this->selected_shape_ = shape;
}
void ShapeManager::hoverShape(const Point &mousePos) {
  Shape *shape = findShape(this->shapes_, [mousePos](Shape *sp) {
    // 对于每一个要清空hover状态
    sp->setIsHover(false);
    return sp->checkTouchedArea(mousePos) == TouchedArea::Body;
  });
  // 存在被选中的图形，则设置对应的被选中状态
  shape->setIsHover(true);
}
void ShapeManager::moveSelectedShapeTo(int levelIdx) {
  if (levelIdx >= shapes_.size()) {
    return;
  }
  // 获取当前选中的shape指针的索引，
  int idx = -1;
  for (int i = 0; i < shapes_.size(); i++) {
    auto sp = shapes_[i];
    if (sp->is_selected()) {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    return;
  }
  if (idx == levelIdx) {
    return;
  }

}

Shape *findShape(std::vector<Shape *> &list, const std::function<bool(Shape *)> &filter) {
  if (filter == nullptr) {
    return nullptr;
  }
  if (list.empty()) {
    return nullptr;
  }
  for (const auto &item : list) {
    if (filter(item)) {
      return item;
    }
  }
  return nullptr;
}
int findShapeIndex(std::vector<Shape *> &list, const std::function<bool(Shape *)> &filter) {
  if (filter == nullptr) {
    return -1;
  }
  if (list.empty()) {
    return -1;
  }
  for (int i = 0; i < list.size(); ++i) {
    if (filter(list[i])) {
      return i;
    }
  }
  return -1;
}
}

