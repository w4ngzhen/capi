#include "pen.h"

namespace capi {
Pen::Pen() : Pen(1, Color(), PenStyle::SolidLine) {}
Pen::Pen(const Color &color) : Pen(1, color, PenStyle::SolidLine) {}
Pen::Pen(const Color &color, int width) : Pen(width, color, PenStyle::SolidLine) {}
Pen::Pen(int width, const Color &color, PenStyle style)
    : color_(color), pen_style_(style), width_(width) {}
void Pen::setWidth(int w) { this->width_ = w; }
void Pen::setColor(const Color &color) { this->color_ = color; }
void Pen::setPenStyle(PenStyle style) { this->pen_style_ = style; }
int Pen::width() const { return this->width_; }
Color Pen::color() const { return this->color_; }
PenStyle Pen::pen_style() const { return this->pen_style_; }

} // namespace capi
