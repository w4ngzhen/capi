#include "painter_qt_impl.h"
#include "capi_core/base/point.h"
#include "capi_core/base/rect.h"
#include "capi_core/base/size.h"
#include "capi_core/paint/brush.h"
#include "capi_core/paint/image.h"
#include "capi_core/paint/painter.h"
#include "capi_core/paint/pen.h"
#include <QFont>
#include <QFontMetrics>

using namespace capi;

PainterQtImpl::PainterQtImpl(QPainter *p) {
  this->qt_painter_ = p;
}

void PainterQtImpl::DrawRect(const Rect &r) {
  this->qt_painter_->drawRect(r.x(), r.y(), r.w(), r.h());
}
void PainterQtImpl::FillRect(const Rect &r, const Brush &b) {
  this->qt_painter_->fillRect(r.x(), r.y(), r.w(), r.h(), this->Convert(b));
}
void PainterQtImpl::DrawEllipse(const Rect &r) {
  this->qt_painter_->drawEllipse(r.x(), r.y(), r.w(), r.h());
}
void PainterQtImpl::DrawLine(const Point &start, const Point &end) {
  this->qt_painter_->drawLine(start.x(), start.y(), end.x(), end.y());
}
void PainterQtImpl::DrawLine(int x1, int y1, int x2, int y2) {
  this->qt_painter_->drawLine(x1, y1, x2, y2);
}
void PainterQtImpl::DrawText(std::string &text, const Rect &r, int align_mode) {
  QString str = QString::fromStdString(text);
  this->qt_painter_->drawText(r.x(), r.y(), r.w(), r.h(), align_mode, str);
}
void PainterQtImpl::DrawText(std::string &text, const Point &p) {
  QString str = QString::fromStdString(text);
  this->qt_painter_->drawText(p.x(), p.y(), str);
}
void PainterQtImpl::DrawImage(const Rect &canvas_rect, const Image *img) {
  auto *p_img_qt_impl = (ImageQtImpl *) img;
  QImage q_img = *p_img_qt_impl->GetQImage();
  this->qt_painter_->drawImage(PainterQtImpl::Convert(canvas_rect), q_img);
}

void PainterQtImpl::DrawImage(const Rect &canvas_rect, const Image *img,
                              const Rect &img_rect) {
  auto *p_img_qt_impl = (ImageQtImpl *) img;
  QImage q_img = *p_img_qt_impl->GetQImage();
  this->qt_painter_->drawImage(PainterQtImpl::Convert(canvas_rect), q_img,
                               PainterQtImpl::Convert(img_rect));
}

void PainterQtImpl::SetColorReverse(bool reverse) {
  if (reverse) {
    this->qt_painter_->setPen(QPen(Qt::white));
    this->qt_painter_->setCompositionMode(
        QPainter::RasterOp_SourceAndNotDestination);
  } else {
    this->qt_painter_->setCompositionMode(QPainter::CompositionMode_Source);
  }
}

Size PainterQtImpl::MeasureTextSize(std::string &text, std::string font_family,
                                    int font_size) {

  QString q_text = QString::fromStdString(text);
  QFont font(q_text, font_size);
  QFontMetrics fm(font);
  const QRect temp_rect = fm.boundingRect(q_text);
  return {temp_rect.width(), temp_rect.height()};
}
void PainterQtImpl::SetPen(const Pen &p) {
  this->qt_painter_->setPen(this->Convert(p));
}
void PainterQtImpl::SetBrush(const Brush &b) {
  this->qt_painter_->setBrush(this->Convert(b));
}
void PainterQtImpl::SetFont(std::string font_family, int font_size) {
  this->qt_painter_->setFont(
      QFont(QString::fromStdString(font_family), font_size));
}
void PainterQtImpl::Save() { this->qt_painter_->save(); }
void PainterQtImpl::Restore() { this->qt_painter_->restore(); }

QRect PainterQtImpl::Convert(const Rect &r) {
  return {r.x(), r.y(), r.w(), r.h()};
}
QSize PainterQtImpl::Convert(const Size &s) { return {s.w(), s.h()}; }
QPoint PainterQtImpl::Convert(const Point &p) { return {p.x(), p.y()}; }
QColor PainterQtImpl::Convert(const Color &c) {
  return {c.r(), c.g(), c.b(), c.a()};
}
QBrush PainterQtImpl::Convert(const Brush &b) {
  return {this->Convert(b.color())};
}
QPen PainterQtImpl::Convert(const Pen &p) {
  Qt::PenStyle qt_pen_style = Qt::SolidLine;
  switch (p.pen_style()) {
    case capi::PenStyle::SolidLine:qt_pen_style = Qt::SolidLine;
      break;
    case capi::PenStyle::DotLine:qt_pen_style = Qt::DotLine;
      break;
    case capi::PenStyle::DashLine:qt_pen_style = Qt::DashLine;
      break;
    default:break;
  }
  QPen pen(qt_pen_style);
  pen.setColor(this->Convert(p.color()));
  pen.setWidth(p.width());
  return pen;
}

PainterQtImpl::~PainterQtImpl() {
  this->qt_painter_ = nullptr;
}
