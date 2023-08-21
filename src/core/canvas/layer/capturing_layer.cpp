#include <iostream>
#include <sstream>

#include "capturing_layer.h"
#include "core/global/global.h"
#include "core/paint/brush.h"
#include "core/paint/color.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"
#include "core/utils/math_utils.h"

namespace capi {
CapturingLayer::CapturingLayer(const Size &canvasSize) : Layer(canvasSize) {}

void CapturingLayer::paint(Painter *painter) {
  painter->save();

  // === 渲染数据准备 ===
  int cX = mouse_current_pos_.x();
  int cY = mouse_current_pos_.y();

  int sX = mouse_press_pos_.x();
  int sY = mouse_press_pos_.y();
  // 准备绘制的矩形
  Rect rect = math_utils::calcRect(cX, cY, sX, sY);

  // === 绘制阶段 ===
  // 1. 绘制当前的矩形框
  painter->setPen(Pen(Color(0, 111, 222), 1));
  painter->drawRect(rect);
  // 2. 绘制矩形框宽高的信息
  CapturingLayer::drawCaptureRectInfo(painter, this->mouse_press_pos_,
                                      this->mouse_current_pos_);

  painter->restore();
}

void CapturingLayer::finishCapture(const Point &endPos) {
  Point startPos = this->mouse_press_pos_;
  int w = abs(startPos.x() - endPos.x());
  int h = abs(startPos.y() - endPos.y());
  if (w == 0 || h == 0) {
    return;
  }
  // 获取区域，触发事件完成事件。
  Rect capturedRect =
      math_utils::calcRect(endPos.x(), endPos.y(), startPos.x(), startPos.y());
}

void CapturingLayer::drawCaptureRectInfo(Painter *painter, Point &startPos,
                                         Point &endPos) {
  int fontSize = 10;
  std::string fontFamily = "SimHei";

  int w = abs(startPos.x() - endPos.x());
  int h = abs(startPos.y() - endPos.y());

  std::stringstream ss;
  ss << w << " x " << h;
  std::string rectSizeInfo = ss.str();
  Size textSize = painter->mesureTextSize(rectSizeInfo, fontFamily, fontSize);

  // 计算文本的背景尺寸（比起文本size要多一个padding）
  int PADDING = 5;
  Size bgRectSize(textSize.w() + PADDING * 2, textSize.h() + PADDING * 2);

  int GAP = 10;

  // 接下来，根据鼠标所在的位置以及拖动的方向，决定显示信息的背景矩形所在位置
  // 获取以鼠标按下的位置为起点，当前所在位置为终点计算向量，得到拖动的向量方向
  Point vector(endPos.x() - startPos.x(), endPos.y() - startPos.y());
  Point bgRectPos;
  // todo 暂未排除x/y为零的情况
  if (vector.x() > 0) {
    if (vector.y() > 0) {
      // 右下方向
      bgRectPos.setX(endPos.x() + GAP);
      bgRectPos.setY(endPos.y() + GAP);
    } else {
      // 右上方向
      bgRectPos.setX(endPos.x() + GAP);
      bgRectPos.setY(endPos.y() - bgRectSize.h() - GAP);
    }
  } else {
    if (vector.y() > 0) {
      // 左下方向
      bgRectPos.setX(endPos.x() - bgRectSize.w() - GAP);
      bgRectPos.setY(endPos.y() + GAP);
    } else {
      // 左上方向
      bgRectPos.setX(endPos.x() - bgRectSize.w() - GAP);
      bgRectPos.setY(endPos.y() - bgRectSize.h() - GAP);
    }
  }

  // 进行绘制操作
  painter->save();
  // 填充背景矩形
  Rect bgRect(bgRectPos, bgRectSize);
  painter->fillRect(bgRect, Brush(Color(244, 224, 126)));
  // 修改画笔颜色
  painter->setPen(Pen(Color(0, 0, 0)));
  // 绘制背景矩形边框
  painter->drawRect(bgRect);
  // 绘制文本
  painter->setFont(fontFamily, fontSize);

  painter->drawText(rectSizeInfo, bgRect,
                    AlignMode::Center | AlignMode::VCenter);
  painter->restore();
}

} // namespace capi
