#pragma once

#include <vector>
#include <functional>
#include "shape.h"

namespace capi {

enum ShapeType {
  Rect = 0,
  Ellipse
};

class ShapeManager {
  /**
   * 构造函数
   */
  ShapeManager();
  /**
   * 增加一个图形
   * @param selected 增加后是否立即选中
   */
  void addShape(bool selected);

  /**
   * 使用鼠标点击选择某个图形
   * @param mousePos
   */
  void selectShape(const Point &mousePos);
  /**
   * 使用鼠标悬浮到某个图形
   * @param mousePos
   */
  void hoverShape(const Point &mousePos);
  /**
   * 将正在选择的图形移动到指定索引层
   * @param levelIdx
   */
  void moveSelectedShapeTo(int levelIdx);
  /**
 * 移除正在选择的图形
 */
  void removeSelectedShape();
private:
  /**
   * 已有的所有图形
   */
  std::vector<Shape *> shapes_;
  /**
   * 当前正选择的图形
   */
  Shape *selected_shape_;
};

/**
 * 根据filter获取对应的Shape指针
 */
Shape *findShape(std::vector<Shape *> &list, const std::function<bool(Shape *)> &filter = nullptr);
/**
 * 根据filter获取对应的Shape的索引
 */
int findShapeIndex(std::vector<Shape *> &list, const std::function<bool(Shape *)> &filter = nullptr);
}
