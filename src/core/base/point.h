#pragma once

namespace capi {
class Point {
public:
  Point(int x, int y);
  Point() = default;
  Point(Point &&) = default;
  Point(const Point &) = default;
  Point &operator=(Point &&) = default;
  Point &operator=(const Point &) = default;
  ~Point() = default;

  int x();
  int y();
  void setX(int);
  void setY(int);

private:
  int x_;
  int y_;
};
} // namespace capi
