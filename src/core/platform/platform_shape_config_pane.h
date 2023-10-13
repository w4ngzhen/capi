#pragma once
#include "core/layer/shape/shape_config.h"
#include <functional>
namespace capi {

/**
* 图形配置变动事件
*/
typedef std::function<void(const ShapeConfig &config)> OnShapeConfigChangeEventCb;

class PlatformShapeConfigPane {
  virtual void Show() = 0;
  virtual void Hide() = 0;
  void SetShapeConfig(const ShapeConfig &config);
  ShapeConfig shape_config() const;
  void SetOnShapeConfigChangeEventCb(OnShapeConfigChangeEventCb cb);
 private:
  ShapeConfig config_;
  OnShapeConfigChangeEventCb on_shape_config_change_event_cb_;
};
}