#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <QFont>
#include <QFontMetrics>
#include <QRect>

namespace math_helper {

/**
 * @brief calcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
QRect calcRect(int x1, int y1, int x2, int y2);

/**
 * @brief calcTextSize 计算在给定字体下的文本像素尺寸
 */
bool calcTextSize(const QString &text, const QFont &font, QSize *size);

/**
 * @brief effectiveSize Size的宽度和高度是否超过指定的长度
 * 例如 (w = 10, h = 2) 就没超过length = 3
 */
bool sizeLengthOver(const QSize &size, const int length);
/**
 * @brief 判断某点是否出于Rect的有效范围内
 */
bool posInEffectiveRect(const QPoint &pos, const QRect &rect,
                        const int borderWidth);
/**
 * 将基于逻辑像素的Rect转为实际像素的Rect
 * PS：存在像素精度损失
 * @param logicRect
 * @param scaleW
 * @param scaleH
 */
QRect rectLogicPixelToRealPixel(QRect logicRect, int scaleW, int scaleH);
/**
 * 根据圆心坐标得到指定半径的圆的矩形
 * @param x
 * @param y
 */
QRect getCircleRectByPoint(int x, int y, int r);
} // namespace math_helper

#endif // MATH_HELPER_H
