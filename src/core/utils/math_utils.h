#pragma once

#include "core/global/global.h"
#include "core/layer/captured/captured_layer_common.h"

namespace capi::math_utils {

/**
 * @brief calcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
Rect calcRect(int x1, int y1, int x2, int y2);
/**
 * 根据圆心坐标得到指定半径的圆的矩形
 * @param x
 * @param y
 */
Rect getCircleRectByPoint(int x, int y, int radius);
/**
 * @brief 判断某点是否出于Rect的有效范围内
 */
bool posInEffectiveRect(const Point &pos, const Rect &rect,
                        int borderWidth);
/**
 * 将一个矩形扩大一定像素
 * @param origin
 * @param extendSize 延伸的尺寸，即整个矩形每一侧都扩大一定尺寸
 * @return
 */
Rect enlargeRect(const Rect &origin, int extendSize);
/**
 * 根据原始矩形，以及拖动偏移值，拖动的对应角位置，计算对应拖动后的矩形数据
 * @param origin
 * @param dx
 * @param dy
 * @param part
 * @param targetRect 结果矩形指针，需要外部给入
 * @return false，计算失败，true，计算成功
 */
bool calcCornerDragRect(const Rect &originRect, int dx, int dy, ShapePart part, Rect *targetRect);
/**
 * 封装逻辑判断是否是四个角部分
 * @param part
 * @return
 */
bool checkIsCornerPart(ShapePart part);
} // namespace capi::math_utils

