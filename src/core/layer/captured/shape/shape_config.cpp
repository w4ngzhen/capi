#include "shape_config.h"

namespace capi {
ShapeConfig::ShapeConfig() : ShapeConfig(Color(), Color(), 1, false) {

}
ShapeConfig::ShapeConfig(const Color &bc, const Color &pc, int pw, bool isFilled)
    : brush_color_(bc), pen_color_(pc), pen_width_(pw), is_filled_(isFilled) {
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
bool ShapeConfig::is_filled() const {
  return is_filled_;
}
void ShapeConfig::setIsFilled(bool is_filled) {
  is_filled_ = is_filled;
}

}