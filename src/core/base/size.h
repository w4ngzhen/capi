#pragma once

namespace capi {
class Size {
public:
  Size(int w, int h);
  Size() = default;
  Size(Size &&) = default;
  Size(const Size &) = default;
  Size &operator=(Size &&) = default;
  Size &operator=(const Size &) = default;
  ~Size() = default;

  [[nodiscard]] int w() const;
  [[nodiscard]] int h() const;
  void SetW(int);
  void SetH(int);

private:
  int w_;
  int h_;
};

} // namespace capi
