#pragma once

class Point;
class Size;

namespace capi {
class Rect {
public:
  bool contains(Point &) const;
  bool contains(int x, int y) const;

  int x() const;
  int y() const;
  Point pos() const;

  int w() const;
  int h() const;
  Size size() const;

  void setX(int x);
  void setY(int y);
  void setPos(Point &);

  void setW(int w);
  void setH(int h);
  void setSize(Size &);

  Rect(int x, int y, int w, int h);
  Rect(Point &, Size &);

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
