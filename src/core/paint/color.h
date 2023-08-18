namespace capi {
class Color {
public:
  Color();
  Color(Color &&) = default;
  Color(const Color &) = default;
  Color &operator=(Color &&) = default;
  Color &operator=(const Color &) = default;
  ~Color();

  Color(int r, int g, int b);

private:
  int r_;
  int g_;
  int b_;
};

} // namespace capi
