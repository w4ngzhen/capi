#pragma once

namespace capi {
class Point {
public:
  Point(int x, int y);
  Point();
  Point(Point &&) = default;
  Point(const Point &) = default;
  Point &operator=(Point &&) = default;
  Point &operator=(const Point &) = default;
  ~Point() = default;

  [[nodiscard]] int x() const;
  [[nodiscard]] int y() const;
  void SetX(int);
  void SetY(int);

private:
  int x_;
  int y_;
};
} // namespace capi
