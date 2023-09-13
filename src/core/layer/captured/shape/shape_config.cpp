#include "shape_config.h"

namespace capi {
ShapeConfig::ShapeConfig() : ShapeConfig(Color(), Color(), 1) {

}
ShapeConfig::ShapeConfig(const Color &bc, const Color &pc, int pw) : brush_color_(bc), pen_color_(pc), pen_width_(pw) {
}

void ShapeConfig::setBrushColor(const Color &bc) {
  brush_color_ = bc;
}
void ShapeConfig::setPenColor(const Color &pc) {
  pen_color_ = pc;
}
void ShapeConfig::setPenWidth(int pw) {
  pen_width_ = pw;
}
Color ShapeConfig::brush_color() const {
  return brush_color_;
}

Color ShapeConfig::pen_color() const {
  return pen_color_;
}

int ShapeConfig::pen_width() const {
  return pen_width_;
}

}