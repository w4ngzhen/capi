#include "image_qt_impl.h"
#include <QImage>

ImageQtImpl::ImageQtImpl(QImage *img) { this->qt_img_ = img; }
capi::Color ImageQtImpl::colorAt(int x, int y) {
  QColor qColor = this->qt_img_->pixel(x, y);
  return {qColor.red(), qColor.green(), qColor.blue(), qColor.alpha()};
}
capi::Size ImageQtImpl::size() {
  auto qSize = this->qt_img_->size();
  return {qSize.width(), qSize.height()};
}
QImage *ImageQtImpl::getQImage() const { return this->qt_img_; }
