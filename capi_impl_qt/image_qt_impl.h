#pragma once

#include "capi_core/paint/image.h"
#include <QImage>

using namespace capi;

class QImage;

class ImageQtImpl : public capi::Image {
public:
  capi::Color GetColorAt(int x, int y) override;
  capi::Size size() override;

  // impl
  explicit ImageQtImpl(QImage *img);
  [[nodiscard]] QImage *GetQImage() const;

private:
  QImage *qt_img_;
};
