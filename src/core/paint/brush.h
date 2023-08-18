#include "color.h"

namespace capi {
class Brush {
public:
  Brush();
  Brush(Brush &&) = default;
  Brush(const Brush &) = default;
  Brush &operator=(Brush &&) = default;
  Brush &operator=(const Brush &) = default;
  ~Brush();

  Brush(Color &);
  void setColor(Color &);

private:
  Color color_;
};
} // namespace capi
