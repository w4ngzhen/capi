#pragma once

#include "core/global/global.h"
#include "core/layer/captured/captured_layer_common.h"

namespace capi::math_utils {

/**
 * @brief CalcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
Rect CalcRect(int x1, int y1, int x2, int y2);
/**
 * 根据坐标得到指定厚度的正方形
 * @param x
 * @param y
 * @param size 最终的正方形宽度为该 size ，由于存在中心1个像素，建议奇数值
 */
Rect GetSquareByPoint(int x, int y, int size);
/**
 * @brief 判断某点是否出于Rect的有效范围内
 */
bool CheckPosInEffectiveRect(const Point &pos, const Rect &rect,
                             const int border_width);
/**
 * 将一个矩形扩大一定像素
 * @param origin
 * @param extend_size 延伸的尺寸，即整个矩形每一侧都扩大一定尺寸
 * @return
 */
Rect EnlargeRect(const Rect &origin, int extend_size);
/**
 * 根据原始矩形，以及拖动偏移值，拖动的对应角位置，计算对应拖动后的矩形数据
 * @param origin
 * @param dx
 * @param dy
 * @param part
 * @param target_rect 结果矩形指针，需要外部给入
 * @return false，计算失败，true，计算成功
 */
bool CalcCornerDragRect(const Rect &origin_rect, int dx, int dy, ShapePart part, Rect *target_rect);
/**
 * 封装逻辑判断是否是四个角部分
 * @param part
 * @return
 */
bool CheckIsCornerPart(ShapePart part);
} // namespace capi::math_utils

