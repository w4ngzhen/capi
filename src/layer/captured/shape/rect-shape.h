#pragma once

#include <QBrush>
#include <QPen>
#include <QRect>

#include "shape.h"

class RectShape : public Shape {
public:
  void onPaint(QPainter *) override;
  void setBody(QRect);

private:
  QRect body_;
};
