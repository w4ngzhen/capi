#ifndef PAINT_HELPER_H
#define PAINT_HELPER_H

#include <QPainter>
#include <QRect>

namespace paint_helper {

/**
 * @brief paintCapturingRectSizeTip 绘制图片捕获的过程中正在捕获的图片的size
 */
    void paintCapturingRectSizeTip(QPainter &painter,
                                   const QPoint &mousePos,
                                   const QPoint &mouseDownPos,
                                   const QSize &capturingRect);

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
}


#endif // PAINT_HELPER_H
