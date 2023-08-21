#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"
#include "core/paint/brush.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"

#include <QPainter>

using namespace capi;

class PainterQtImpl : public Painter {
  void drawRect(const Rect &) override;
  void fillRect(const Rect &, const Brush &) override;
  void drawEllipse(const Rect &) override;
  void drawLine(const Point &start, const Point &end) override;
  void drawLine(int x1, int y1, int x2, int y2) override;
  void drawText(std::string &, const Rect &, int alignMode) override;
  void drawText(std::string &, const Point &) override;
  void drawText(std::string &) override;
  void drawImage(Image *img, const Rect &) override;
  void setColorReverse(bool) override;
  Size mesureTextSize(std::string &, std::string fontFamily,
                      int fontSize) override;
  void setPen(const Pen &) override;
  void setBrush(const Brush &) override;
  void setFont(std::string fontFamily, int fontSize) override;
  void save() override;
  void restore() override;

  QSize convert(const Size &);
  QPoint convert(const Point &);
  QRect convert(const Rect &);
  QColor convert(const Color &);
  QBrush convert(const Brush &);
  QPen convert(const Pen &);

private:
  QPainter *qt_painter_;
};
