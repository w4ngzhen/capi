#include "color.h"
namespace capi {
Color::Color(int r, int g, int b, int a) : r_(r), g_(g), b_(b), a_(a) {}
Color::Color() : Color(255, 255, 255, 255) {}
Color::Color(int r, int g, int b) : Color(r, g, b, 255) {}
int Color::r() const { return this->r_; }
int Color::g() const { return this->g_; }
int Color::b() const { return this->b_; }
int Color::a() const { return this->a_; }
} // namespace capi
