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

}


#endif // PAINT_HELPER_H
