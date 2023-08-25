#pragma once

namespace capi {

class Point;
class Size;

class Rect {
public:
  [[nodiscard]] bool contains(const Point &) const;
  [[nodiscard]] bool contains(int x, int y) const;

  [[nodiscard]] int x() const;
  [[nodiscard]] int y() const;
  [[nodiscard]] Point pos() const;

  [[nodiscard]] int w() const;
  [[nodiscard]] int h() const;
  [[nodiscard]] Size size() const;

  void setX(int x);
  void setY(int y);
  void setPos(Point &);

  void setW(int w);
  void setH(int h);
  void setSize(Size &);

  Rect(int x, int y, int w, int h);
  Rect(const Point &, const Size &);

  Rect() = default;
  Rect(Rect &&) = default;
  Rect(const Rect &) = default;
  Rect &operator=(Rect &&) = default;
  Rect &operator=(const Rect &) = default;
  ~Rect() = default;

private:
  int x_;
  int y_;
  int w_;
  int h_;
};

} // namespace capi
