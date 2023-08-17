#pragma once

#include "shape.h"

// 线的端点的类型
enum AnchorType {
  None = 0,
  Arrow,
};

class LineShape : public Shape {
public:

  void onPaint(QPainter *) override;
  bool isMouseHover(QMouseEvent *) override;

  void setStartPos(QPoint pos) { this->start_pos_ = pos; }
  void setEndPos(QPoint pos) { this->end_pos_ = pos; }
  void setStartAnchorType(AnchorType type) { this->start_anchor_type_ = type; }
  void setEndAnchorType(AnchorType type) { this->end_anchor_type_ = type; }

private:
  QPoint start_pos_;
  QPoint end_pos_;
  AnchorType start_anchor_type_;
  AnchorType end_anchor_type_;
};
