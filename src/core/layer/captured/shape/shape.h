#pragma once
#include "core/base/rect.h"
#include "core/paint/color.h"
#include "core/paint/brush.h"
#include "core/base/point.h"
#include "shape_config.h"
#include "core/layer/captured/captured_layer_common.h"

namespace capi {

class Painter;

class Shape {

protected:
  /**
   * 构造函数
   * @param config 图形绘制配置
   */
  explicit Shape(const ShapeConfig &config);
  /**
   * 默认配置
   */
  explicit Shape();
  /**
   * 纯虚函数
   * 每一个shape必须实现图形内容的渲染
   */
  virtual void onContentPaint(Painter *) = 0;
  /**
   * 绘制边框
   * 默认实现：
   * 若图形被选中，则会在以content rect作为基准，绘制一个灰色的虚线边框和四个拖拽角图形
   * 若图形被鼠标悬浮，则会在以content rect作为基准，只绘制一个高亮虚线
   * 出上述状态外，不进行任何的绘制。
   */
  virtual void onBorderPaint(Painter *);

public:
  /**
   * 每一个Shape子类都需要实现type getter
   * 一般用于某些场景下，对某个Shape的具体子类要进行特别处理的场景
   * 0xFF01 - 0xFFFF 为保留类型编号
   */
  virtual int type() = 0;
  /**
   * 绘图
   * 默认实现：先 onBorderPaint ，再 onContentPaint
   */
  virtual void onPaint(Painter *);
  /**
   * 判断鼠标能否“碰”到当前图形上的位置哪个位置：
   * 点不到、点到图形本身（拖动会用到）、点到图像边框四个角（resize会用到）
   * @param mousePos
   */
  [[nodiscard]] virtual ShapePart checkPart(const Point &mousePos) const;
  /**
   * getter 图形是否被选中
   */
  [[nodiscard]] bool is_selected() const;
  /**
   * setter：设置是否处于选中状态
   */
  void setIsSelected(bool);
  /**
   * setter：设置是否处于被鼠标悬浮态
   */
  void setIsHover(bool);
  /**
   * getter: 是否线图形
   * 对于图形有一个特例：线（箭头等形式）
   * 对于线，会有相关不一样的操作，例如无法resize，只能拖动开始、结束点
   * 默认实现return false，如果是“线”类图形，需要单独override，返回true
   */
  [[nodiscard]] virtual bool is_line_shape() const;
  /**
   * getter: 开始点
   */
  [[nodiscard]] const Point &start_pos() const;
  /**
   * setter: 设置开始点
   */
  void setStartPos(const Point &);
  /**
   * getter: 获取结束点
   */
  [[nodiscard]] const Point &end_pos() const;
  /**
   * setter: 设置结束点
   */
  void setEndPos(const Point &);

  /**
   * getter: 通过startPos和endPos构造的content矩形
   */
  [[nodiscard]] Rect content_rect() const;

  /**
   * 移动图形的位置
   * @param dx x的偏移量
   * @param dy y的偏移量
   */
  void movePosition(int dx, int dy);

  /**
   * 析构函数
   */
  virtual ~Shape();

protected:
  /**
   * 每一个图形理论上都会占据一个区域，
   * 我们使用两个点（startPos和endPos）来表示这个图形所在区域的矩形
   * 这个区域我们取名为：content_rect，内容矩形区
   * 例如：
   * 若图形是一个矩形，那么这两个点所组成的矩形就是这个矩形的位置
   * 若图形是一个椭圆（圆是特殊椭圆），那么这两个点所组成的矩形就是这个椭圆的外切
   * 若图形是一个线，那么我们可以直接使用 startPos_ 和 endPos_来代表这个线段
   * 若图形是一个文字，那么我们使用这个内容矩形区作为文字填充的区域即可。
   */
  Point startPos_ = Point(10, 10);
  /**
   * 结束点
   */
  Point endPos_ = Point(100, 100);
  /**
   * 图形是否被选中
   */
  bool is_selected_ = false;
  /**
   * 鼠标是否正悬浮在该图形上
   */
  bool is_hover_ = false;
  /**
   * 图形绘制配置
   */
  ShapeConfig config_;
};
}