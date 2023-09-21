#pragma once

namespace capi {

class Point;
class Size;

class Rect {
public:
  [[nodiscard]] bool Contains(const Point &) const;
  [[nodiscard]] bool Contains(int x, int y) const;

  [[nodiscard]] int x() const;
  [[nodiscard]] int y() const;
  [[nodiscard]] Point pos() const;

  [[nodiscard]] int w() const;
  [[nodiscard]] int h() const;
  [[nodiscard]] Size size() const;

  void SetX(int x);
  void SetY(int y);
  void SetPos(Point &);

  void SetW(int w);
  void SetH(int h);
  void SetSize(Size &);

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
