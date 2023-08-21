#pragma once

namespace capi {
class Color {
public:

  Color();
  Color(int r, int g, int b);
  Color(int r, int g, int b, int a);

  int r() const;
  int g() const;
  int b() const;
  int a() const;

private:
  int r_;
  int g_;
  int b_;
  int a_;
};

} // namespace capi
