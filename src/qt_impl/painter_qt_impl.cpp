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

PainterQtImpl::PainterQtImpl(QPainter *p) {
 this->qt_painter_ = p; 
}

void PainterQtImpl::drawRect(const Rect &r) {
  this->qt_painter_->drawRect(r.x(), r.y(), r.w(), r.h());
}
void PainterQtImpl::fillRect(const Rect &r, const Brush &b) {
  this->qt_painter_->fillRect(r.x(), r.y(), r.w(), r.h(), this->convert(b));
}
void PainterQtImpl::drawEllipse(const Rect &r) {
  this->qt_painter_->drawEllipse(r.x(), r.y(), r.w(), r.h());
}
void PainterQtImpl::drawLine(const Point &start, const Point &end) {
  this->qt_painter_->drawLine(start.x(), start.y(), end.x(), end.y());
}
void PainterQtImpl::drawLine(int x1, int y1, int x2, int y2) {
  this->qt_painter_->drawLine(x1, y1, x2, y2);
}
void PainterQtImpl::drawText(std::string &text, const Rect &r, int alignFlag) {
  QString str = QString::fromStdString(text);
  this->qt_painter_->drawText(r.x(), r.y(), r.w(), r.h(), alignFlag, str);
}
void PainterQtImpl::drawText(std::string &text, const Point &p) {
  QString str = QString::fromStdString(text);
  this->qt_painter_->drawText(p.x(), p.y(), str);
}
void PainterQtImpl::drawImage(const Rect &canvasRect, const Image *img) {
  auto *pImgQtImpl = (ImageQtImpl *)img;
  QImage qImg = *pImgQtImpl->getQImage();
  this->qt_painter_->drawImage(PainterQtImpl::convert(canvasRect), qImg);
}

void PainterQtImpl::drawImage(const Rect &canvasRect, const Image *img,
                              const Rect &imgRect) {
  auto *pImgQtImpl = (ImageQtImpl *)img;
  QImage qImg = *pImgQtImpl->getQImage();
  this->qt_painter_->drawImage(PainterQtImpl::convert(canvasRect), qImg,
                               PainterQtImpl::convert(imgRect));
}

void PainterQtImpl::setColorReverse(bool reverse) {
  if (reverse) {
    this->qt_painter_->setPen(QPen(Qt::white));
    this->qt_painter_->setCompositionMode(
        QPainter::RasterOp_SourceAndNotDestination);
  } else {
    this->qt_painter_->setCompositionMode(QPainter::CompositionMode_Source);
  }
}

Size PainterQtImpl::measureTextSize(std::string &text, std::string fontFamily,
                                    int fontSize) {

  QString qText = QString::fromStdString(text);
  QFont font(qText, fontSize);
  QFontMetrics fm(font);
  const QRect tempRect = fm.boundingRect(qText);
  return {tempRect.width(), tempRect.height()};
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
  return {r.x(), r.y(), r.w(), r.h()};
}
QSize PainterQtImpl::convert(const Size &s) { return {s.w(), s.h()}; }
QPoint PainterQtImpl::convert(const Point &p) { return {p.x(), p.y()}; }
QColor PainterQtImpl::convert(const Color &c) {
  return {c.r(), c.g(), c.b(), c.a()};
}
QBrush PainterQtImpl::convert(const Brush &b) {
  return {this->convert(b.color())};
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

PainterQtImpl::~PainterQtImpl() {
  this->qt_painter_ = nullptr; 
}
