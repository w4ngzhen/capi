
#include "explore_layer.h"
#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"
#include "core/paint/brush.h"
#include "core/paint/color.h"
#include "core/paint/pen.h"


namespace capi {

// 工具面板的size
const int TOOL_PANE_WIDTH = 201;
const int TOOL_PANE_HEIGHT = 351;

ExploreLayer::ExploreLayer(Image *canvasImg, const Size &canvasSize)
    : canvas_img_(canvasImg), Layer(canvasSize) {}

void ExploreLayer::paint(Painter *painter) {

  painter->save();
  // 绘制鼠标所在的位置垂直和水平线
  painter->setPen(Pen(Color(255, 0, 0), 1));
  int mX = this->mouse_current_pos_.x();
  int mY = this->mouse_current_pos_.y();
  // 垂直线
  painter->drawLine(mX, 0, mX, this->canvas_size_.h());
  // 水平线
  painter->drawLine(0, mY, this->canvas_size_.w(), mY);

  this->paintToolPane(painter, mX, mY);

  painter->restore();
}

void ExploreLayer::paintToolPane(Painter *painter, int mouseX, int mouseY) {
  // 画布尺寸
  Size canvasSize = this->canvas_size_;
  // 工具面板默认放置画布于右下角，
  // 但如果光标移入了右下角区域，为了防止挡住，工具面板位置变为左上角
  int panelX = canvasSize.w() - TOOL_PANE_WIDTH;
  int panelY = canvasSize.h() - TOOL_PANE_HEIGHT;
  if (mouseX >= panelX && mouseY >= panelY) {
    // 进入被遮挡区域
    panelX = 0;
    panelY = 0;
  }

  this->paintToolPaneAt(painter, panelX, panelY, mouseX, mouseY);
}
void ExploreLayer::paintToolPaneAt(Painter *painter, int panelX, int panelY,
                                   int mouseX, int mouseY) {

  painter->save();

  // 1. 准备工具面板基本Rect数据
  // 注意，这里我们不考虑缩放问题
  const int TOOL_PANE_X = panelX;
  const int TOOL_PANE_Y = panelY;
  const Rect TOOL_PANE_RECT(TOOL_PANE_X, TOOL_PANE_Y, TOOL_PANE_WIDTH,
                            TOOL_PANE_HEIGHT);

  // 2. 计算光标的实际像素位置
  int mX = mouseX;
  int mY = mouseY;
  // 由于current_cursor_pos_是逻辑坐标，
  // 为了获取图片上对应的值，需要转为实际坐标
  auto picRealSize = this->canvas_img_->size();
  int realMouseX = mX * picRealSize.w() / canvas_size_.w();
  int realMouseY = mY * picRealSize.h() / canvas_size_.h();

  // 3. 所在图片真实像素位置周围30px的区域内容
  // 需要将对应区域转为实际区域，进而获取实际的区域的图像内容
  const int PADDING = 30;
  int left = realMouseX - PADDING;
  int top = realMouseY - PADDING;
  // +1是因为要把光标所在位置的1个像素加上
  int w = PADDING * 2 + 1;
  int h = PADDING * 2 + 1;
  Rect realPicRect = Rect(left, top, w, h);
  // 从中取得对应区域的图像内容
  Image *partialImage = this->canvas_img_->copy(realPicRect);

  // 获取光标指定位置的颜色
  Color colorAtCursorPos = this->canvas_img_->colorAt(realMouseX, realMouseY);

  // === 绘制阶段 ===
  // === 1. 给工具面板绘制1像素的边框 ===
  // 注意绘制边框的时候，如果取值为>=2，需要考虑宽度带来的偏移问题，
  // 因为drawRect的线条是pen的中心像素向两边扩展，为了实现方便建议就用1
  const int BORDER_W = 1;
  painter->setPen(Pen(Color(0, 0, 0), BORDER_W));
  // 先使用白色将整个面板填满
  painter->fillRect(TOOL_PANE_RECT, Brush(Color(255, 255, 255)));
  // 再绘制边框
  painter->drawRect(TOOL_PANE_RECT);

  // === 2. 面板顶部绘制一个存放放大图像的区域 ===
  const int SCALED_IMG_W = TOOL_PANE_WIDTH - (BORDER_W * 2);
  const int SCALED_IMG_H = SCALED_IMG_W;
  const int SCALED_IMG_X = TOOL_PANE_X + BORDER_W;
  const int SCALED_IMG_Y = TOOL_PANE_Y + BORDER_W;
  painter->drawImage(partialImage, Rect(SCALED_IMG_X, SCALED_IMG_Y,
                                        SCALED_IMG_W, SCALED_IMG_H));

  // === 3. 在图像放大区域绘制一个中央十字 ===
  // SCALED_IMG_W为奇数
  const int IMG_CENTER_POINT_X = SCALED_IMG_X + (SCALED_IMG_W - 1) / 2;
  const int IMG_CENTER_POINT_Y = SCALED_IMG_Y + (SCALED_IMG_W - 1) / 2;
  // 设置反色模式
  painter->setColorReverse(true);
  painter->setPen(Pen(Color(255, 255, 255), 2));
  // 设置十字路径并绘制
  const int EXPAND_LENGTH = 20;
  Point p1(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y - EXPAND_LENGTH);
  Point p2(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y + EXPAND_LENGTH);
  Point p3(IMG_CENTER_POINT_X - EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  Point p4(IMG_CENTER_POINT_X + EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  painter->drawLine(p1, p2);
  painter->drawLine(p3, p4);
  // 绘制反色十字完成后，一定要将反色模式取消，
  painter->setColorReverse(false);

  // === 4. 在下方涂写十字中心捕获的颜色 ===
  // 4.1 区域中填充颜色
  const int COLOR_DISPLAY_RECT_W = SCALED_IMG_W;
  const int COLOR_DISPLAY_RECT_H = 25;
  const int COLOR_DISPLAY_RECT_X = SCALED_IMG_X;
  const int COLOR_DISPLAY_RECT_Y = SCALED_IMG_Y + SCALED_IMG_H + BORDER_W;
  Rect colorDisplayRect(COLOR_DISPLAY_RECT_X, COLOR_DISPLAY_RECT_Y,
                        COLOR_DISPLAY_RECT_W, COLOR_DISPLAY_RECT_H);
  painter->fillRect(colorDisplayRect, Brush(colorAtCursorPos));
  //
  // 4.2 使用反色，在区域右侧填充色彩信息
  std::string rgbDesc = "RGB DESC";
  // 使用反色
  painter->setColorReverse(true);
  painter->drawText(rgbDesc, colorDisplayRect,
                    AlignMode::VCenter | AlignMode::Right);
  painter->setColorReverse(false);

  // === 5. 在色彩卡下方绘制坐标，以及RGB文字 ===
  // 5.1 准备好区域
  const int INFO_DESC_RECT_W = SCALED_IMG_W;
  const int INFO_DESC_RECT_H = 40;
  const int INFO_DESC_RECT_X = SCALED_IMG_X;
  const int INFO_DESC_RECT_Y = COLOR_DISPLAY_RECT_Y + COLOR_DISPLAY_RECT_H;
  Rect infoDescRect(INFO_DESC_RECT_X, INFO_DESC_RECT_Y, INFO_DESC_RECT_W,
                    INFO_DESC_RECT_H);
  // 5.2 准备好文字
  std::string posDesc = "POS DESC";
  painter->setPen(Pen(Color(0, 0, 0)));
  painter->drawText(posDesc, infoDescRect,
                    AlignMode::Center | AlignMode::VCenter);

  // finally
  painter->restore();
}

} // namespace capi
