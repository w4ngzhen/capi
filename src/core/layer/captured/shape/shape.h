#pragma once
#include "core/base/rect.h"
#include "core/paint/color.h"
#include "core/paint/brush.h"

namespace capi {

class Painter;
class Point;

/**
 * 触碰区域枚举，checkTouchedArea返回
 * 表明鼠标所在的位置，正处于当前图形的哪个区域
 */
enum TouchedArea {
  None = 0,
  Body,
  LeftTop,
  RightTop,
  LeftBottom,
  RightBottom,
};

class Shape {

protected:
  /**
   * 构造函数
   * @param bc 画刷颜色
   * @param pc 画笔颜色
   * @param pw 画笔宽度
   */
  Shape(const Color &bc, const Color &pc, int pw);
  /**
   * 纯虚函数
   * 每一个shape必须实现body的渲染
   */
  virtual void onBodyPaint(Painter *) = 0;
  /**
   * 绘制边框
   * 默认实现：
   * 若图形被选中，则会在body周围绘制一个灰色的虚线边框和四个拖拽角图形
   * 若图形被鼠标悬浮，则会在通过body矩形绘制一个灰色边框
   * 出上述状态外，不进行任何的绘制。
   */
  virtual void onBorderPaint(Painter *);

public:
  /**
   * 绘图
   * 默认实现：先 onBorderPaint ，再 onBodyPaint
   */
  virtual void onPaint(Painter *);
  /**
   * 判断鼠标能否“碰”到当前图形上的位置哪个位置：
   * 点不到、点到图形本身（拖动会用到）、点到图像边框四个角（resize会用到）
   * @param mousePos
   */
  [[nodiscard]] virtual TouchedArea checkTouchedArea(const Point &mousePos) const;
  /**
   * getter 图形是否被选中
   */
  [[nodiscard]] bool is_selected() const;
  /**
   * getter：当前图形的body
   */
  [[nodiscard]] Rect &body();
  /**
   * setter：设置是否处于选中状态
   */
  void setIsSelected(bool);
  /**
   * setter：设置是否处于被鼠标悬浮态
   */
  void setIsHover(bool);
  /**
   * setter：设置画刷颜色
   */
  void setBrushColor(const Color &);
  /**
   * setter：设置画笔颜色
   */
  void setPenColor(const Color &);
  /**
   * setter：设置笔线宽度
   */
  void setPenWidth(int);

protected:
  /**
   * 每个图形都有一个基于Rect的body，
   * 若是一个矩形图形，则body就可以作为这个矩形的形状
   * 若是一个椭圆或圆，则body就是这个椭圆的外切
   */
  Rect body_ = Rect(10, 10, 300, 300);
  /**
   * 图形是否被选中
   */
  bool is_selected_ = false;
  /**
   * 鼠标是否正悬浮在该图形上
   */
  bool is_hover_ = false;
  /**
   * 画刷颜色
   */
  Color brush_color_;
  /**
   * 画笔颜色
   */
  Color pen_color_;
  /**
   * 画笔线宽
   */
  int pen_width_;
};
}