#pragma once

#include "core/global/global.h"
#include "core/paint/image.h"
#include <string>

namespace capi {

class Rect;
class Pen;
class Brush;

class Painter {
public:

  virtual void drawRect(const Rect &) = 0;
  virtual void fillRect(const Rect &, const Brush &) = 0;
  virtual void drawEllipse(const Rect &) = 0;
  virtual void drawLine(const Point &start, const Point &end) = 0;
  virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
  virtual void drawText(std::string &, const Rect &, int alignMode) = 0;
  virtual void drawText(std::string &, const Point &) = 0;

  virtual void drawImage(const Rect &canvasRect, const Image *img) = 0;
  virtual void drawImage(const Rect &canvasRect, const Image *img, const Rect &imgRect) = 0;

  virtual void setColorReverse(bool) = 0;
  virtual Size measureTextSize(std::string &, std::string fontFamily,
                               int fontSize) = 0;
  virtual void setPen(const Pen &) = 0;
  virtual void setBrush(const Brush &) = 0;
  virtual void setFont(std::string fontFamily, int fontSize) = 0;
  virtual void save() = 0;
  virtual void restore() = 0;
  virtual ~Painter() = 0;
private:
};

} // namespace capi
