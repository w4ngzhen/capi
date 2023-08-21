#include "painter_qt_impl.h"
#include "core/base/point.h"
#include "core/base/rect.h"
#include "core/base/size.h"
#include "core/paint/brush.h"
#include "core/paint/image.h"
#include "core/paint/painter.h"
#include "core/paint/pen.h"
#include <QFont>
#include <QFontMetrics>

using namespace capi;

void PainterQtImpl::drawRect(const Rect &r) {
  this->qt_painter_->drawRect(this->convert(r));
}
void PainterQtImpl::fillRect(const Rect &r, const Brush &b) {
  this->qt_painter_->fillRect(this->convert(r), this->convert(b));
}
void PainterQtImpl::drawEllipse(const Rect &r) {
  this->qt_painter_->drawEllipse(this->convert(r));
}
void PainterQtImpl::drawLine(const Point &start, const Point &end) {
  this->qt_painter_->drawLine(this->convert(start), this->convert(end));
}
void PainterQtImpl::drawLine(int x1, int y1, int x2, int y2) {
  this->qt_painter_->drawLine(x1, y1, x2, y2);
}
void PainterQtImpl::drawText(std::string &, const Rect &, int alignMode) {}
void PainterQtImpl::drawText(std::string &, const Point &) {}
void PainterQtImpl::drawText(std::string &) {}
void PainterQtImpl::drawImage(Image *img, const Rect &) {}
void PainterQtImpl::setColorReverse(bool reverse) {
  if (reverse) {
    this->qt_painter_->setPen(QPen(Qt::white));
    this->qt_painter_->setCompositionMode(
        QPainter::RasterOp_SourceAndNotDestination);
  } else {
    this->qt_painter_->setCompositionMode(QPainter::CompositionMode_Source);
  }
}
Size PainterQtImpl::mesureTextSize(std::string &text, std::string fontFamily,
                                   int fontSize) {

  QString qText = QString::fromStdString(text);
  QFont font(qText, fontSize);
  QFontMetrics fm(font);
  const QRect tempRect = fm.boundingRect(qText);
  return Size(tempRect.width(), tempRect.height());
}
void PainterQtImpl::setPen(const Pen &p) {
  this->qt_painter_->setPen(this->convert(p));
}
void PainterQtImpl::setBrush(const Brush &b) {
  this->qt_painter_->setBrush(this->convert(b));
}
void PainterQtImpl::setFont(std::string fontFamily, int fontSize) {
  this->qt_painter_->setFont(
      QFont(QString::fromStdString(fontFamily), fontSize));
}
void PainterQtImpl::save() { this->qt_painter_->save(); }
void PainterQtImpl::restore() { this->qt_painter_->restore(); }

QRect PainterQtImpl::convert(const Rect &r) {
  return QRect(r.x(), r.y(), r.w(), r.h());
}
QSize PainterQtImpl::convert(const Size &s) { return QSize(s.w(), s.h()); }
QPoint PainterQtImpl::convert(const Point &p) { return QPoint(p.x(), p.y()); }
QColor PainterQtImpl::convert(const Color &c) {
  return QColor(c.r(), c.g(), c.b(), c.a());
}
QBrush PainterQtImpl::convert(const Brush &b) {
  return QBrush(this->convert(b.color()));
}
QPen PainterQtImpl::convert(const Pen &p) {
  Qt::PenStyle qtPenStyle = Qt::SolidLine;
  switch (p.pen_style()) {
  case capi::PenStyle::SolidLine:
    qtPenStyle = Qt::SolidLine;
    break;
  case capi::PenStyle::DotLine:
    qtPenStyle = Qt::DotLine;
    break;
  case capi::PenStyle::DashLine:
    qtPenStyle = Qt::DashLine;
    break;
  default:
    break;
  }
  QPen pen(qtPenStyle);
  pen.setColor(this->convert(p.color()));
  pen.setWidth(p.width());
  return pen;
}
