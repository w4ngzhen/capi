#pragma once

#include "core/paint/color.h"
namespace capi {
/**
 * 图形相关配置，例如是否填充、画布颜色等配置单独使用类管理
 */
class ShapeConfig {
public:
  ShapeConfig();
  /**
 * 构造函数
 * @param bc 画刷颜色
 * @param pc 画笔颜色
 * @param pw 画笔宽度
 * @param is_filled 图形是否填充
 */
  ShapeConfig(const Color &bc, const Color &pc, int pw, bool is_filled);
  /**
   * 默认构造函数
   */
  ShapeConfig(const ShapeConfig &) = default;
  /**
 * setter：设置画刷颜色
 */
  void SetBrushColor(const Color &);
  /**
   * setter：设置画笔颜色
   */
  void SetPenColor(const Color &);
  /**
   * setter：设置笔线宽度
   */
  void SetPenWidth(int);
  /**
   * setter: 设置图形是否填充
   * @param is_filled
   */
  void SetIsFilled(bool is_filled);
  /**
   * getter: 画刷颜色
   */
  [[nodiscard]] Color brush_color() const;

  /**
   * getter: 画笔颜色
   */
  [[nodiscard]] Color pen_color() const;

  /**
   * 画笔线条宽度
   */
  [[nodiscard]] int pen_width() const;

  /**
   * getter: 图形是否填充
   * @return
   */
  [[nodiscard]] bool is_filled() const;

private:
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
  /**
   * 是否填充
   */
  bool is_filled_;
};
}

