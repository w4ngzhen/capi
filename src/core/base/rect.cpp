#include "point.h"
#include "size.h"
#include "rect.h"
namespace capi {
Rect::Rect(int x, int y, int w, int h) : x_(x), y_(y), w_(w), h_(h) {}
Rect::Rect(const Point &p, const Size &s) : x_(p.x()), y_(p.y()), w_(s.w()), h_(s.h()) {}

int Rect::x() const { return this->x_; }
int Rect::y() const { return this->y_; }
int Rect::w() const { return this->w_; }
int Rect::h() const { return this->h_; }
void Rect::setX(int x) { this->x_ = x; }
void Rect::setY(int y) { this->y_ = y; }
void Rect::setW(int w) { this->w_ = w; }
void Rect::setH(int h) { this->h_ = h; }
void Rect::setPos(Point &p) {
  this->x_ = p.x();
  this->y_ = p.y();
}
void Rect::setSize(Size &s) {
  this->w_ = s.w();
  this->h_ = s.h();
}
Point Rect::pos() const { return {this->x_, this->y_}; }
Size Rect::size() const { return {this->w_, this->h_}; }

bool Rect::contains(int x, int y) const {
  return (this->x_ <= x && this->x_ + this->w_ >= x) &&
      (this->y_ <= y && this->y_ + this->h_ >= y);
}
bool Rect::contains(const Point &p) const { return this->contains(p.x(), p.y()); }
} // namespace capi
