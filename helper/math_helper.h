#ifndef MATH_HELPER_H
#define MATH_HELPER_H


#include <QRect>
#include <QFont>
#include <QFontMetrics>

namespace math_helper
{

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
bool posInEffectiveRect(const QPoint &pos, const QRect &rect, const int borderWidth);
}


#endif // MATH_HELPER_H
