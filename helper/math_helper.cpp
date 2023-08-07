
#include "helper/math_helper.h"

namespace math_helper {

/**
 * @brief calcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
QRect calcRect(int x1, int y1, int x2, int y2) {
  int leftTopX, leftTopY, rightBottomX, rightBottomY;
  if (x1 <= x2) {
    leftTopX = x1;
    rightBottomX = x2;
  } else {
    leftTopX = x2;
    rightBottomX = x1;
  }

  if (y1 <= y2) {
    leftTopY = y1;
    rightBottomY = y2;
  } else {
    leftTopY = y2;
    rightBottomY = y1;
  }
  return QRect(leftTopX, leftTopY, rightBottomX - leftTopX,
               rightBottomY - leftTopY);
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
} // namespace math_helper
