#pragma once

#include "capi_core/global.h"
#include "capi_core/paint/image.h"
#include <string>

namespace capi {

class Rect;
class Pen;
class Brush;

class Painter {
public:

  virtual void DrawRect(const Rect &) = 0;
  virtual void FillRect(const Rect &, const Brush &) = 0;
  virtual void DrawEllipse(const Rect &) = 0;
  virtual void DrawLine(const Point &start, const Point &end) = 0;
  virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
  virtual void DrawText(std::string &, const Rect &, int align_mode) = 0;
  virtual void DrawText(std::string &, const Point &) = 0;

  virtual void DrawImage(const Rect &canvas_rect, const Image *img) = 0;
  virtual void DrawImage(const Rect &canvas_rect, const Image *img, const Rect &img_rect) = 0;

  virtual void SetColorReverse(bool) = 0;
  virtual Size MeasureTextSize(std::string &,
                               std::string font_family,
                               int fontSize) = 0;
  virtual void SetPen(const Pen &) = 0;
  virtual void SetBrush(const Brush &) = 0;
  virtual void SetFont(std::string font_family, int font_size) = 0;
  virtual void Save() = 0;
  virtual void Restore() = 0;
  virtual ~Painter() = 0;
private:
};

} // namespace capi
