#ifndef SCREENSHOTUTILS_H
#define SCREENSHOTUTILS_H


#include <QRect>

namespace ScreenShotUtils
{

/**
 * @brief calcRect 给定两个点（x1，y1）（x2，y2），计算对应的矩形
 */
QRect calcRect(int x1, int y1, int x2, int y2)
{
    int leftTopX, leftTopY, rightBottomX, rightBottomY;
    if (x1 <= x2)
    {
        leftTopX = x1;
        rightBottomX = x2;
    }
    else
    {
        leftTopX = x2;
        rightBottomX = x1;
    }

    if (y1 <= y2)
    {
        leftTopY = y1;
        rightBottomY = y2;
    }
    else
    {
        leftTopY = y2;
        rightBottomY = y1;
    }
    return QRect(leftTopX, leftTopY, rightBottomX - leftTopX, rightBottomY - leftTopY);
}

}


#endif // SCREENSHOTUTILS_H
