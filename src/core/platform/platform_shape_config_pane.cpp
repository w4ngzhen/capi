#include "platform_shape_config_pane.h"

namespace capi {
void PlatformShapeConfigPane::SetShapeConfig(const ShapeConfig &config) {
  config_ = config;
}

ShapeConfig PlatformShapeConfigPane::shape_config() const {
  return config_;
}
void PlatformShapeConfigPane::SetOnShapeConfigChangeEventCb(OnShapeConfigChangeEventCb cb) {
  on_shape_config_change_event_cb_ = cb;
}
}