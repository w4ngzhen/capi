#include "canvas_captured_image_save_event.h"
#include "core/base/rect.h"
namespace capi {
CanvasCapturedImageSaveEvent::CanvasCapturedImageSaveEvent(const Rect &r,
                                                           const SaveMode mode)
    : captured_rect_(r), save_mode_(mode) {}
Rect CanvasCapturedImageSaveEvent::capured_rect() const {
  return this->captured_rect_;
}
SaveMode CanvasCapturedImageSaveEvent::save_mode() const {
  return this->save_mode_;
}
} // namespace capi
