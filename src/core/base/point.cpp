#include "point.h"
namespace capi {
Point::Point() : x_(0), y_(0) {}
Point::Point(int x, int y) {
  this->x_ = x;
  this->y_ = y;
}
int Point::x() const { return this->x_; }
int Point::y() const { return this->y_; }
void Point::setX(int x) { this->x_ = x; }
void Point::setY(int y) { this->y_ = y; }
} // namespace capi
