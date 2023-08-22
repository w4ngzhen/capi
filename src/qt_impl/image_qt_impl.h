#pragma once

#include "core/paint/image.h"
#include <QImage>

using namespace capi;

class QImage;

class ImageQtImpl : public capi::Image {
public:
  capi::Color colorAt(int x, int y) override;
  capi::Size size() override;

  // impl
  explicit ImageQtImpl(QImage *img);
  QImage *getQImage() const;

private:
  QImage *qt_img_;
};
