#include "point.h"
namespace capi {
Point::Point(int x, int y) {
  this->x_ = x;
  this->y_ = y;
}
int Point::x() { return this->x_; }
int Point::y() { return this->y_; }
void Point::setX(int x) { this->x_ = x; }
void Point::setY(int y) { this->y_ = y; }
} // namespace capi
