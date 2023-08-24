#pragma once

#include "core/base/rect.h"
namespace capi {

enum SaveMode {
  Clipboard = 0,
  File
};

class CanvasCapturedImageSaveEvent {

public:
  CanvasCapturedImageSaveEvent(const Rect &capturedRect, SaveMode);
  [[nodiscard]] Rect captured_rect() const;
  [[nodiscard]] SaveMode save_mode() const;
private:
  const Rect captured_rect_;
  const SaveMode save_mode_;
};
} // namespace capi
