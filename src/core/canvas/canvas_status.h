#pragma once

namespace capi {

enum CanvasStatus {

  // 光标在画布上移动，但未进行截图
  Explore = 0,
  // 正在截图中
  Capturing,
  // 截图完毕后的渲染
  Captured
};
}
