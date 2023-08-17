#pragma once

#include <QBrush>
#include <QColor>
#include <QPen>

class QPainter;
class QMouseEvent;

// 涉及直线，边框的图像，线的类型
// 普通实线
// 虚线
enum LineType { Solid = 0, Dash = 1 };

class Shape {
public:
  Shape();
  ~Shape();

  virtual void onPaint(QPainter *) = 0;
  virtual bool isMouseHover(QMouseEvent *) = 0;

  void setIsFilled(bool val) { this->isFilled_ = val; }
  void setColor(QColor color) {
    this->color_ = color;
    this->brush_.setColor(color);
    this->pen_.setColor(color);
  }
  void setPenWidth(uint penWidth) {
    this->pen_width_ = penWidth;
    this->pen_.setWidth(penWidth);
  }
  void setLineType(LineType lineType) {
    this->line_type_ = lineType;
    switch (line_type_) {
    case Dash:
      this->pen_.setStyle(Qt::DashLine);
    case Solid:
      this->pen_.setStyle(Qt::SolidLine);
      break;
    default:
      break;
    }
  }

protected:
  bool isFilled_;
  uint pen_width_;

  QColor color_;
  QBrush brush_;
  QPen pen_;
  LineType line_type_;
};

inline Shape::~Shape() {}
