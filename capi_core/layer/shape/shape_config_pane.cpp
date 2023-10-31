#include "capi_core/layer/shape/shape_config_pane.h"

namespace capi {
void ShapeConfigPane::OnPaint(capi::Painter *painter) {

  painter->Save();

  painter->Restore();
}

void ShapeConfigPane::OnMousePress(capi::Point &pos) {

}

void ShapeConfigPane::SetPosition(capi::Point &pos) {
  pos_ = pos;
}
}
