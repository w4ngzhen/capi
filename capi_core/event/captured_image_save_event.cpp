#include "capi_core/event/captured_image_save_event.h"
#include "capi_core/base/rect.h"
namespace capi {
CapturedImageSaveEvent::CapturedImageSaveEvent(const Rect &r,
                                               const SaveMode mode)
    : captured_rect_(r), save_mode_(mode) {}
Rect CapturedImageSaveEvent::captured_rect() const {
  return this->captured_rect_;
}
SaveMode CapturedImageSaveEvent::save_mode() const {
  return this->save_mode_;
}
} // namespace capi
