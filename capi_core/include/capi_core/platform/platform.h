#include "capi_core/platform//platform_shape_config_pane.h"
namespace capi {
class Platform {
  virtual PlatformShapeConfigPane *GetOsShapeConfigPane() = 0;
};
}