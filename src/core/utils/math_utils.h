#pragma once

#include "core/global/global.h"

namespace capi {
namespace math_utils {

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
                        const int borderWidth);
} // namespace math_utils
} // namespace capi
