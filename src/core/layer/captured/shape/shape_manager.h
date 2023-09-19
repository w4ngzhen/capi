#pragma once

#include <vector>
#include <functional>
#include "shape.h"
#include "core/global/global.h"

namespace capi {

enum ShapeType {
  Rectangle = 0,
  Ellipse
};

class ShapeManager {
public:
  /**
   * 构造函数
   */
  ShapeManager();
  /**
  * 增加指定类型，指定配置的图形
  * @param selected 增加后是否立即选中
  */
  void addShape(ShapeType, const ShapeConfig &, bool selected);
  /**
  * 移除正在选择的图形
  */
  void deleteSelectedShape();
  /**
   * 鼠标按下
   */
  void onMousePress(const Point &);
  /**
   * 鼠标移动
   */
  void onMouseMove(const Point &);
  /**
   * 鼠标释放
   */
  void onMouseRelease(const Point &);
  /**
   * 绘制
   */
  void onPaint(Painter *);
private:
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
   * 已有的所有图形
   */
  std::vector<Shape *> shapes_;
  /**
   * 存储当前正被选中的图形
   */
  Shape *selected_shape_ = nullptr;
  /**
   * 描述当前被选择图形正被拖动的部分
   * 根据该值决定鼠标按下、移动等操作达到对图形拖动的效果
   */
  ShapePart selected_shape_dragging_part_ = None;
  /**
   * 记录鼠标按下位置
   */
  Point mouse_press_pos_;
  /**
   * 记录鼠标当前移动位置
   */
  Point mouse_current_pos_;
};
}
