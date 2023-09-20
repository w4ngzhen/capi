#include "captured_layer.h"
#include "core/base/point.h"
#include "core/utils/math_utils.h"
#include "core/layer/captured/shape/captured_shape.h"
#include "core/layer/captured/shape/rect_shape.h"
namespace capi {
CapturedLayer::CapturedLayer(const Size &canvasSize) : Layer(canvasSize) {
  selected_shape_dragging_part_ = None;
  mouse_press_pos_ = Point();
  // 根据被捕获的矩形信息，构造一个CapturedShape，默认选中
  auto capSp = new CapturedShape();
  shapes_.push_back(capSp);
}

void CapturedLayer::setCapturedRect(const Rect &capturedRect) {
  auto capSp = getCapturedShape();
  if (capSp != nullptr) {
    auto startPos = capturedRect.pos();
    auto endPos = Point(capturedRect.x() + capturedRect.w(),
                        capturedRect.y() + capturedRect.h());
    capSp->setStartPos(startPos);
    capSp->setEndPos(endPos);
    // 并默认选中
    capSp->setIsSelected(true);
    selected_shape_ = capSp;
  }
}

void CapturedLayer::addShape(ShapeType type, const ShapeConfig &config, bool selected) {
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
void CapturedLayer::deleteSelectedShape() {
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
void CapturedLayer::selectShape(const Point &mousePos) {
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
void CapturedLayer::hoverShape(const Point &mousePos) {
  Shape *lastHoverSp = nullptr;
  for (auto &sp : this->shapes_) {
    if (sp->checkPart(mousePos) == Body) {
      if (lastHoverSp != nullptr) {
        lastHoverSp->setIsHover(false);
      }
      sp->setIsHover(true);
      lastHoverSp = sp;
    }
  }
}
void CapturedLayer::moveSelectedShapeToTargetLevel(int levelIdx) {

  if (selected_shape_ == nullptr) {
    return;
  }

  /**
   * CapturedShape 始终处于图层索引为0的位置
   * 无法移动 CapturedShape，以及将任何其他的图形移动到 CapturedShape 的下层
   */
  if (levelIdx == 0 || selected_shape_->type() == 0xFFFF) {
    return;
  }

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
void CapturedLayer::onMousePress(const Point &mousePos) {
  Shape *tempSelectedSp = nullptr;
  for (auto &sp : this->shapes_) {
    auto part = sp->checkPart(mousePos);
    // 如果点击到了Body或四个角，我们就认为点击到了该图形
    if (part == Body || math_utils::checkIsCornerPart(part)) {
      selected_shape_dragging_part_ = part;
      // 点击到了某个图形的body，则该图形此刻要被选中，
      // 但在选中前，需要先判断是不是已经有了被选中的图形
      // 如有则先把前面被视为选中的图形置为false
      if (tempSelectedSp != nullptr) {
        tempSelectedSp->setIsSelected(false);
      }
      sp->setIsSelected(true);
      tempSelectedSp = sp;
    } else {
      sp->setIsSelected(false);
    }
  }
  if (tempSelectedSp == nullptr) {
    // 没有点击到任何的图形，则清理成员变量 selected_shape_ 的状态，并置为nullptr
    if (selected_shape_ != nullptr) {
      selected_shape_->setIsSelected(false);
      selected_shape_ = nullptr;
    }
  } else {
    // 有点击到某个图形，那么这里不需要对 selected_shape_ 状态清理
    // 因为上面遍历的流程已经做了，这里只需要赋值即可
    selected_shape_ = tempSelectedSp;
  }
}
void CapturedLayer::onMouseMove(const Point &mousePos) {

  // 保存鼠标上一次位置，并记录更新当前位置
  auto lastMousePos = mouse_current_pos_;
  mouse_current_pos_ = mousePos;

  /**
   * 情况：不处于拖动态，或没有被选择的图形
   * 不进行任何的拖动操作，只需要更新hover效果
   */
  if (selected_shape_dragging_part_ == None || selected_shape_ == nullptr) {
    hoverShape(mousePos);
    return;
  }

  /**
   * 情况：且有被选择的图形，且正被拖动
   */
  // 先计算拖动距离
  int dx = mouse_current_pos_.x() - lastMousePos.x();
  int dy = mouse_current_pos_.y() - lastMousePos.y();
  // 拖动整个图形，则整体移动
  if (selected_shape_dragging_part_ == Body) {
    selected_shape_->movePosition(dx, dy);
    return;
  }
  // 拖动四个角 或 “线”图形拖动开始、结束节点，就要处理resize
  auto &sp = selected_shape_->start_pos();
  auto &ep = selected_shape_->end_pos();
  auto isLineShape = selected_shape_->is_line_shape();

  //“线”图形，只能拖动开始、结束端点
  if (isLineShape) {
    if (selected_shape_dragging_part_ == LineStart) {
      selected_shape_->setStartPos(Point(sp.x() + dx, sp.y() + dy));
      return;
    } else if (selected_shape_dragging_part_ == LineEnd) {
      selected_shape_->setEndPos(Point(ep.x() + dx, ep.y() + dy));
      return;
    }
    return;
  }
  // 处理非“线”图形，譬如矩形、圆等，它们以矩形作为承载，四个角都能移动
  auto currContentRect = selected_shape_->content_rect();
  if (math_utils::checkIsCornerPart(selected_shape_dragging_part_)) {
    // 使用工具方法计算四个角的拖动
    Rect targetRect{};
    auto calcOk = math_utils::calcCornerDragRect(
        currContentRect,
        dx, dy,
        selected_shape_dragging_part_,
        &targetRect);
    if (!calcOk) {
      return;
    }
    auto newStartPos = Point(targetRect.x(), targetRect.y());
    auto newEndPos = Point(newStartPos.x() + targetRect.w(), newStartPos.y() + targetRect.h());
    selected_shape_->setStartPos(newStartPos);
    selected_shape_->setEndPos(newEndPos);
  }

}
void CapturedLayer::onMouseRelease(const Point &mousePos) {
  // 松开鼠标，则关闭拖动状态
  selected_shape_dragging_part_ = None;
}
void CapturedLayer::onMouseDoubleClick(const Point &) {
  // 准备构造captured canvas_image 保存事件
  // 暂时只支持保存到粘贴板
  // 从集合中找到CapturedShape，获取该Shape的矩形数据
  auto capSp = getCapturedShape();
  if (capSp == nullptr) {
    return;
  }
  CapturedImageSaveEvent ev(capSp->content_rect(), SaveMode::Clipboard);
  this->layer_event_on_captured_layer_image_save_cb_(&ev);
}

void CapturedLayer::onPaint(Painter *painter) {
  // 将各个图形进行绘制
  for (auto &sp : shapes_) {
    sp->onPaint(painter);
  }
}

void CapturedLayer::onKeyPress(Key k, KeyboardModifier m) {
  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->layer_event_on_quit_current_layer_cb_();
    return;
  }
  if (k == Key::Key_Delete) {
    // 删除操作
    deleteSelectedShape();
    return;
  }
  if (k == Key::Key_A) {
    addShape(Rectangle, ShapeConfig(), true);
    return;
  }
  if (k == Key::Key_Q) {
    addShape(Rectangle, ShapeConfig(), false);
    return;
  }
}

void CapturedLayer::setLayerEventOnCapturedLayerImageSaveCb(LayerEventOnCapturedLayerImageSaveCb cb) {
  this->layer_event_on_captured_layer_image_save_cb_ = std::move(cb);
}
void CapturedLayer::onCanvasResize(const Size &size) {
  Layer::onCanvasResize(size);
  auto capSp = getCapturedShape();
  if (capSp != nullptr) {
    dynamic_cast<CapturedShape *>(capSp)->setCanvasSize(size);
  }
}
Shape *CapturedLayer::getCapturedShape() {
  auto capSpItr = std::find_if(
      shapes_.begin(),
      shapes_.end(),
      [](Shape *sp) {
        return sp->type() == 0xFFFF;
      }
  );
  if (capSpItr != shapes_.end()) {
    return *capSpItr;
  }
  return nullptr;
}

}

