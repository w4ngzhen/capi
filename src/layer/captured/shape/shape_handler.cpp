#include "shape_handler.h"
#include "shape.h"

#include <QMouseEvent>
#include <QPainter>

ShapeHandler::ShapeHandler() {}

void ShapeHandler::onPaint(QPainter *painter) {
  uint size = this->shapes_.size();
  for (int i = 0; i < size; i++) {
    Shape *shape = this->shapes_.at(i);
    shape->onPaint(painter);
  }
}

void ShapeHandler::mousePressEvent(QMouseEvent *event) {
  // 点击在两种场景下会发生：
  // 1. 没有选中任何一个shape
  // 2. 已经选中了某一个shape，拖动它之前的点击
}

ShapeHandler::~ShapeHandler() {}
