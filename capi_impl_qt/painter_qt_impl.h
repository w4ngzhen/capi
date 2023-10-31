#include "capi_core/base/point.h"
#include "capi_core/base/rect.h"
#include "capi_core/base/size.h"
#include "capi_core/paint/brush.h"
#include "capi_core/paint/image.h"
#include "capi_core/paint/painter.h"
#include "image_qt_impl.h"

#include <QPainter>

using namespace capi;

class PainterQtImpl : public Painter {

 public:
  explicit PainterQtImpl(QPainter *qPainter);

  void DrawRect(const Rect &) override;
  void FillRect(const Rect &, const Brush &) override;
  void DrawEllipse(const Rect &) override;
  void DrawLine(const Point &start, const Point &end) override;
  void DrawLine(int x1, int y1, int x2, int y2) override;
  void DrawText(std::string &, const Rect &, int align_mode) override;
  void DrawText(std::string &, const Point &) override;
  void DrawImage(const Rect &canvas_rect, const Image *img) override;
  void DrawImage(const Rect &canvas_rect, const Image *img,
                 const Rect &img_rect) override;
  void SetColorReverse(bool) override;
  Size MeasureTextSize(std::string &,
                       std::string font_family,
                       int font_size) override;
  void SetPen(const Pen &) override;
  void SetBrush(const Brush &) override;
  void SetFont(std::string font_family, int font_size) override;
  void Save() override;
  void Restore() override;

  static QSize Convert(const Size &);
  static QPoint Convert(const Point &);
  static QRect Convert(const Rect &);
  static QColor Convert(const Color &);
  QBrush Convert(const Brush &);
  QPen Convert(const Pen &);

  ~PainterQtImpl() override;
 private:
  QPainter *qt_painter_;
};
