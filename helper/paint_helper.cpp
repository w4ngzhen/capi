#include "helper/paint_helper.h"
#include "helper/math_helper.h"

namespace paint_helper {

/**
 * @brief paintCapturingRectSizeTip 绘制图片捕获的过程中正在捕获的图片的size
 */
void paintCapturingRectSizeTip(QPainter &painter, const QPoint &mousePos,
                               const QPoint &mouseDownPos,
                               const QSize &capturingRect) {

  QString rectSizeTip;
  rectSizeTip.append(QString::number(capturingRect.width()))
      .append(" x ")
      .append(QString::number(capturingRect.height()));
  // 根据字体计算文本的size
  QFont font("SimHei", 10);
  QSize textSize;
  if (!math_helper::calcTextSize(rectSizeTip, font, &textSize)) {
    return;
  };

  // 计算文本的背景尺寸（比起文本size要多一个padding）
  const int PADDING = 5;
  QSize bgRectSize(textSize.width() + PADDING * 2,
                   textSize.height() + PADDING * 2);

  const int GAP = 10;

  // 接下来，根据鼠标所在的位置以及拖动的方向，决定显示信息的背景矩形所在位置
  // 获取以鼠标按下的位置为起点，当前所在位置为终点计算向量，得到拖动的向量方向
  QPoint vector = mousePos - mouseDownPos;
  QPoint bgRectPos;
  // todo 暂未排除x/y为零的情况
  if (vector.x() > 0) {
    if (vector.y() > 0) {
      // 右下方向
      bgRectPos.setX(mousePos.x() + GAP);
      bgRectPos.setY(mousePos.y() + GAP);
    } else {
      // 右上方向
      bgRectPos.setX(mousePos.x() + GAP);
      bgRectPos.setY(mousePos.y() - bgRectSize.height() - GAP);
    }
  } else {
    if (vector.y() > 0) {
      // 左下方向
      bgRectPos.setX(mousePos.x() - bgRectSize.width() - GAP);
      bgRectPos.setY(mousePos.y() + GAP);
    } else {
      // 左上方向
      bgRectPos.setX(mousePos.x() - bgRectSize.width() - GAP);
      bgRectPos.setY(mousePos.y() - bgRectSize.height() - GAP);
    }
  }
  // 进行绘制操作
  painter.save();
  // 填充背景矩形
  QRect bgRect(bgRectPos, bgRectSize);
  painter.fillRect(bgRect, QBrush(QColor(244, 224, 126)));
  // 修改画笔颜色
  painter.setPen(QPen(QColor(0, 0, 0)));
  // 绘制背景矩形边框
  painter.drawRect(bgRect);
  // 绘制文本
  painter.setFont(font);
  painter.drawText(bgRect.x() + PADDING,
                   bgRect.y() + bgRect.height() - PADDING - 3, rectSizeTip);
  painter.restore();
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

} // namespace paint_helper
