
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

ExploreLayer::ExploreLayer(Image *canvasImg, const Size &canvas_size)
    : canvas_img_(canvasImg), Layer(canvas_size) {}

void ExploreLayer::OnPaint(Painter *painter) {

  painter->Save();
  // 绘制鼠标所在的位置垂直和水平线
  painter->SetPen(Pen(Color(255, 0, 0), 1));
  int m_x = this->mouse_current_pos_.x();
  int m_y = this->mouse_current_pos_.y();
  // 垂直线
  painter->DrawLine(m_x, 0, m_x, this->canvas_size_.h());
  // 水平线
  painter->DrawLine(0, m_y, this->canvas_size_.w(), m_y);

  this->paintToolPane(painter, m_x, m_y);

  painter->Restore();
}

void ExploreLayer::paintToolPane(Painter *painter, int mouse_x, int mouse_y) {
  // 画布尺寸
  Size canvas_size = this->canvas_size_;
  // 工具面板默认放置画布于右下角，
  // 但如果光标移入了右下角区域，为了防止挡住，工具面板位置变为左上角
  int panel_x = canvas_size.w() - TOOL_PANE_WIDTH;
  int panel_y = canvas_size.h() - TOOL_PANE_HEIGHT;
  if (mouse_x >= panel_x && mouse_y >= panel_y) {
    // 进入被遮挡区域
    panel_x = 0;
    panel_y = 0;
  }

  this->paintToolPaneAt(painter, panel_x, panel_y, mouse_x, mouse_y);
}
void ExploreLayer::paintToolPaneAt(Painter *painter, int panel_x, int panel_y,
                                   int mouse_x, int mouse_y) {

  painter->Save();

  // 1. 准备工具面板基本Rect数据
  // 注意，这里我们不考虑缩放问题
  const int TOOL_PANE_X = panel_x;
  const int TOOL_PANE_Y = panel_y;
  const Rect TOOL_PANE_RECT(TOOL_PANE_X, TOOL_PANE_Y, TOOL_PANE_WIDTH,
                            TOOL_PANE_HEIGHT);

  // 2. 计算光标的实际像素位置
  // 由于current_cursor_pos_是逻辑坐标，
  // 为了获取图片上对应的值，需要转为实际坐标
  auto picRealSize = this->canvas_img_->size();
  int realMouseX = mouse_x * picRealSize.w() / canvas_size_.w();
  int realMouseY = mouse_y * picRealSize.h() / canvas_size_.h();

  // 3. 所在图片真实像素位置周围30px的区域内容
  // 需要将对应区域转为实际区域，进而获取实际的区域的图像内容
  const int PADDING = 30;
  int left = realMouseX - PADDING;
  int top = realMouseY - PADDING;
  // +1是因为要把光标所在位置的1个像素加上
  int w = PADDING * 2 + 1;
  int h = PADDING * 2 + 1;
  // 从而区域图片真实区域
  Rect real_pic_rect = Rect(left, top, w, h);

  // 获取光标指定位置的颜色
  Color color_at_cursor_pos = this->canvas_img_->GetColorAt(realMouseX, realMouseY);

  // === 绘制阶段 ===
  // === 1. 给工具面板绘制1像素的边框 ===
  // 注意绘制边框的时候，如果取值为>=2，需要考虑宽度带来的偏移问题，
  // 因为drawRect的线条是pen的中心像素向两边扩展，为了实现方便建议就用1
  const int BORDER_W = 1;
  painter->SetPen(Pen(Color(0, 0, 0), BORDER_W));
  // 先使用白色将整个面板填满
  painter->FillRect(TOOL_PANE_RECT, Brush(Color(255, 255, 255)));
  // 再绘制边框
  painter->DrawRect(TOOL_PANE_RECT);

  // === 2. 面板顶部绘制一个存放放大图像的区域 ===
  const int SCALED_IMG_W = TOOL_PANE_WIDTH - (BORDER_W * 2);
  const int SCALED_IMG_H = SCALED_IMG_W;
  const int SCALED_IMG_X = TOOL_PANE_X + BORDER_W;
  const int SCALED_IMG_Y = TOOL_PANE_Y + BORDER_W;
  // 在该区域绘制图片：
  // 从图片（this->canvas_img_）的指定区域（real_pic_rect）取出图像，
  // 绘制到画布指定区域（第一个Rect参数）
  painter->DrawImage(
      Rect(SCALED_IMG_X, SCALED_IMG_Y, SCALED_IMG_W, SCALED_IMG_H),
      this->canvas_img_, real_pic_rect);

  // === 3. 在图像放大区域绘制一个中央十字 ===
  // SCALED_IMG_W为奇数
  const int IMG_CENTER_POINT_X = SCALED_IMG_X + (SCALED_IMG_W - 1) / 2;
  const int IMG_CENTER_POINT_Y = SCALED_IMG_Y + (SCALED_IMG_W - 1) / 2;
  // 设置反色模式
  painter->SetColorReverse(true);
  painter->SetPen(Pen(Color(255, 255, 255), 2));
  // 设置十字路径并绘制
  const int EXPAND_LENGTH = 20;
  Point p1(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y - EXPAND_LENGTH);
  Point p2(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y + EXPAND_LENGTH);
  Point p3(IMG_CENTER_POINT_X - EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  Point p4(IMG_CENTER_POINT_X + EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  painter->DrawLine(p1, p2);
  painter->DrawLine(p3, p4);
  // 绘制反色十字完成后，一定要将反色模式取消，
  painter->SetColorReverse(false);

  // === 4. 在下方涂写十字中心捕获的颜色 ===
  // 4.1 区域中填充颜色
  const int COLOR_DISPLAY_RECT_W = SCALED_IMG_W;
  const int COLOR_DISPLAY_RECT_H = 25;
  const int COLOR_DISPLAY_RECT_X = SCALED_IMG_X;
  const int COLOR_DISPLAY_RECT_Y = SCALED_IMG_Y + SCALED_IMG_H + BORDER_W;
  Rect color_display_rect(COLOR_DISPLAY_RECT_X, COLOR_DISPLAY_RECT_Y,
                          COLOR_DISPLAY_RECT_W, COLOR_DISPLAY_RECT_H);
  painter->FillRect(color_display_rect, Brush(color_at_cursor_pos));
  //
  // 4.2 使用反色，在区域右侧填充色彩信息
  std::string rgb_desc = "RGB DESC";
  // 使用反色
  painter->SetColorReverse(true);

  painter->DrawText(rgb_desc, color_display_rect,
                    AlignFlag::AlignVCenter | AlignFlag::AlignRight);
  painter->SetColorReverse(false);

  // === 5. 在色彩卡下方绘制坐标，以及RGB文字 ===
  // 5.1 准备好区域
  const int INFO_DESC_RECT_W = SCALED_IMG_W;
  const int INFO_DESC_RECT_H = 40;
  const int INFO_DESC_RECT_X = SCALED_IMG_X;
  const int INFO_DESC_RECT_Y = COLOR_DISPLAY_RECT_Y + COLOR_DISPLAY_RECT_H;
  Rect info_desc_rect(INFO_DESC_RECT_X, INFO_DESC_RECT_Y, INFO_DESC_RECT_W,
                      INFO_DESC_RECT_H);
  // 5.2 准备好文字
  std::string pos_desc = "POS DESC";
  painter->SetPen(Pen(Color(0, 0, 0)));
  painter->DrawText(pos_desc, info_desc_rect,
                    AlignFlag::AlignVCenter | AlignFlag::AlignRight);

  // finally
  painter->Restore();
}

void ExploreLayer::OnKeyPress(Key k, KeyboardModifier m) {

  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->layer_event_on_quit_current_layer_cb_();
    return;
  }

  // 上下左右控制鼠标位置精准移动
  int dx = 0;
  int dy = 0;
  switch (k) {
    case Key::Key_Up:dy -= 1;
      break;
    case Key::Key_Down:dy += 1;
      break;
    case Key::Key_Left:dx -= 1;
      break;
    case Key::Key_Right:dx += 1;
      break;
    default:break;
  }
  auto mousePos = this->mouse_current_pos_;
  this->mouse_current_pos_.SetX(mousePos.x() + dx);
  this->mouse_current_pos_.SetY(mousePos.y() + dy);
}

} // namespace capi
