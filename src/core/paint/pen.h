#include "../global/global.h"
#include "color.h"

namespace capi {

class Pen {
public:
  Pen();
  Pen(PenStyle);
  Pen(Color &, PenStyle);
  Pen(int width, Color &, PenStyle);
  void setWidth(int w);
  void setColor(Color &);
  void setPenStyle(PenStyle);

private:
  int width_;
  Color color_;
  PenStyle pen_style_;
};
} // namespace capi
