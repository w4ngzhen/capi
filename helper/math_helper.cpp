
#include "helper/math_helper.h"
#include <QDebug>
#include <stdlib.h>

namespace math_helper {

/**
 * @brief calcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
QRect calcRect(int x1, int y1, int x2, int y2) {

  qDebug("x1 = %d, y1 = %d, x2 = %d, y2 = %d", x1, y1, x2, y2);
  int x = fmin(x1, x2);
  int y = fmin(y1, y2);
  int w = abs(x1 - x2);
  int h = abs(y1 - y2);
  return QRect(x, y, w, h);
}

/**
 * @brief calcTextSize 计算在给定字体下的文本像素尺寸
 * @param text
 * @param font
 * @param size
 */
bool calcTextSize(const QString &text, const QFont &font, QSize *size) {
  QFontMetrics fm(font);
  const QRect tempRect = fm.boundingRect(text);
  if (size == nullptr) {
    return false;
  }
  size->setWidth(tempRect.width());
  size->setHeight(tempRect.height());
  return true;
}

/**
 * @brief effectiveSize Size的宽度和高度是否超过指定的长度
 * 例如 (w = 10, h = 2) 就没超过length = 10
 * @param size
 * @return
 */
bool sizeLengthOver(const QSize &size, int length) {
  return size.width() >= length && size.height() >= length;
}

bool posInEffectiveRect(const QPoint &pos, const QRect &rect,
                        const int borderWidth) {
  if (borderWidth <= 0) {
    return rect.contains(pos);
  }
  QRect _rect(rect.x() + borderWidth, rect.y() + borderWidth,
              rect.width() - borderWidth * 2, rect.height() - borderWidth * 2);
  return rect.contains(pos);
}

QRect rectLogicPixelToRealPixel(QRect logicRect, int scaleW, int scaleH) {
  int x = logicRect.x() * scaleW;
  int y = logicRect.y() * scaleH;
  int w = logicRect.width() * scaleW;
  int h = logicRect.height() * scaleH;
  return {x, y, w, h};
}

QRect getCircleRectByPoint(int x, int y, int r) {
  int left = x - r;
  int top = y - r;
  int w = r * 2 + 1;
  int h = w;
  return QRect(left, top, w, h);
}
} // namespace math_helper
