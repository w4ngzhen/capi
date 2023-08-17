#pragma once

#include <QBrush>
#include <QPen>
#include <QRect>

#include "shape.h"

class RectShape : public Shape {
public:
  void onPaint(QPainter *) override;
  bool isMouseHover(QMouseEvent *) override;
  void setRect(QRect rect) {
    this->rect_ = rect;
  }

private:
  QRect rect_;
};
