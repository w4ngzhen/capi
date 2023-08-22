#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"
#include "core/paint/brush.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "image_qt_impl.h"

#include <QPainter>

using namespace capi;

class PainterQtImpl : public Painter {

public:
  explicit PainterQtImpl(QPainter *qPainter);

  void drawRect(const Rect &) override;
  void fillRect(const Rect &, const Brush &) override;
  void drawEllipse(const Rect &) override;
  void drawLine(const Point &start, const Point &end) override;
  void drawLine(int x1, int y1, int x2, int y2) override;
  void drawText(std::string &, const Rect &, int alignMode) override;
  void drawText(std::string &, const Point &) override;
  void drawImage(const Rect &canvasRect, const Image *img) override;
  void drawImage(const Rect &canvasRect, const Image *img,
                 const Rect &imgRect) override;
  void setColorReverse(bool) override;
  Size measureTextSize(std::string &, std::string fontFamily,
                       int fontSize) override;
  void setPen(const Pen &) override;
  void setBrush(const Brush &) override;
  void setFont(std::string fontFamily, int fontSize) override;
  void save() override;
  void restore() override;

  static QSize convert(const Size &);
  static QPoint convert(const Point &);
  static QRect convert(const Rect &);
  static QColor convert(const Color &);
  QBrush convert(const Brush &);
  QPen convert(const Pen &);

  ~PainterQtImpl() override;
private:
  QPainter *qt_painter_;
};
