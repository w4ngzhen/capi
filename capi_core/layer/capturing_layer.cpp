#include <iostream>
#include <sstream>
#include <utility>

#include "capi_core/layer/capturing_layer.h"
#include "capi_core/global.h"
#include "capi_core/paint/brush.h"
#include "capi_core/paint/color.h"
#include "capi_core/paint/painter.h"
#include "capi_core/paint/pen.h"
#include "capi_core/utils/math_utils.h"

namespace capi {
CapturingLayer::CapturingLayer(const Size &canvas_size) : Layer(canvas_size) {}

void CapturingLayer::OnPaint(Painter *painter) {
  painter->Save();

  // === 渲染数据准备 ===
  int mouse_curr_x = mouse_current_pos_.x();
  int mouse_curr_y = mouse_current_pos_.y();

  int mouse_press_x = mouse_press_pos_.x();
  int mouse_press_y = mouse_press_pos_.y();
  // 准备绘制的矩形
  Rect rect = math_utils::CalcRect(mouse_curr_x, mouse_curr_y, mouse_press_x, mouse_press_y);

  // === 绘制阶段 ===
  // 1. 绘制当前的矩形框
  painter->SetPen(Pen(Color(0, 111, 222), 1));
  painter->DrawRect(rect);
  // 2. 绘制矩形框宽高的信息
  CapturingLayer::DrawCaptureRectInfo(painter, this->mouse_press_pos_,
                                      this->mouse_current_pos_);

  painter->Restore();
}

void CapturingLayer::OnMouseRelease(const Point &endPos) {
  Point start_pos = this->mouse_press_pos_;
  int w = abs(start_pos.x() - endPos.x());
  int h = abs(start_pos.y() - endPos.y());
  if (w == 0 || h == 0) {
    return;
  }
  // 获取区域，触发事件完成事件。
  Rect captured_rect =
      math_utils::CalcRect(endPos.x(), endPos.y(), start_pos.x(), start_pos.y());

  if (this->layer_event_on_capturing_layer_finish_cb_ != nullptr) {
    (this->layer_event_on_capturing_layer_finish_cb_)(true, captured_rect);
  }
}

void CapturingLayer::DrawCaptureRectInfo(Painter *painter,
                                         Point &start_pos,
                                         Point &current_pos) {
  int font_size = 10;
  std::string font_family = "SimHei";

  int w = abs(start_pos.x() - current_pos.x());
  int h = abs(start_pos.y() - current_pos.y());

  std::stringstream ss;
  ss << w << " x " << h;
  std::string rect_size_info = ss.str();
  Size text_size = painter->MeasureTextSize(rect_size_info, font_family, font_size);

  // 计算文本的背景尺寸（比起文本size要多一个padding）
  int PADDING = 5;
  Size bg_rect_size(text_size.w() + PADDING * 2, text_size.h() + PADDING * 2);

  int GAP = 10;

  // 接下来，根据鼠标所在的位置以及拖动的方向，决定显示信息的背景矩形所在位置
  // 获取以鼠标按下的位置为起点，当前所在位置为终点计算向量，得到拖动的向量方向
  Point vector(current_pos.x() - start_pos.x(), current_pos.y() - start_pos.y());
  Point bg_rect_pos;
  // todo 暂未排除x/y为零的情况
  if (vector.x() > 0) {
    if (vector.y() > 0) {
      // 右下方向
      bg_rect_pos.SetX(current_pos.x() + GAP);
      bg_rect_pos.SetY(current_pos.y() + GAP);
    } else {
      // 右上方向
      bg_rect_pos.SetX(current_pos.x() + GAP);
      bg_rect_pos.SetY(current_pos.y() - bg_rect_size.h() - GAP);
    }
  } else {
    if (vector.y() > 0) {
      // 左下方向
      bg_rect_pos.SetX(current_pos.x() - bg_rect_size.w() - GAP);
      bg_rect_pos.SetY(current_pos.y() + GAP);
    } else {
      // 左上方向
      bg_rect_pos.SetX(current_pos.x() - bg_rect_size.w() - GAP);
      bg_rect_pos.SetY(current_pos.y() - bg_rect_size.h() - GAP);
    }
  }

  // 进行绘制操作
  painter->Save();
  // 填充背景矩形
  Rect bg_rect(bg_rect_pos, bg_rect_size);
  painter->FillRect(bg_rect, Brush(Color(244, 224, 126)));
  // 修改画笔颜色
  painter->SetPen(Pen(Color(0, 0, 0)));
  // 绘制背景矩形边框
  painter->DrawRect(bg_rect);
  // 绘制文本
  painter->SetFont(font_family, font_size);

  painter->DrawText(rect_size_info, bg_rect,
                    AlignFlag::AlignVCenter | AlignFlag::AlignRight);
  painter->Restore();
}

void CapturingLayer::SetLayerEventOnCapturingLayerFinishCb(LayerEventOnCapturingLayerFinishCb cb) {
  this->layer_event_on_capturing_layer_finish_cb_ = std::move(cb);
}

void CapturingLayer::OnKeyPress(Key k, KeyboardModifier m) {
  // ESC 退出当前层
  if (k == Key::Key_Escape) {
    this->layer_event_on_quit_current_layer_cb_();
    return;
  }
}
} // namespace capi
