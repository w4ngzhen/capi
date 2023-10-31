#pragma once

namespace capi {
class Color {
public:

  Color();
  Color(int r, int g, int b);
  Color(int r, int g, int b, int a);

  [[nodiscard]] int r() const;
  [[nodiscard]] int g() const;
  [[nodiscard]] int b() const;
  [[nodiscard]] int a() const;

private:
  int r_;
  int g_;
  int b_;
  int a_;
};

} // namespace capi
