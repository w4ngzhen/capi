
#include "shape.h"
#include <vector>

class QPainter;
class QMouseEvent;

class ShapeHandler {
public:
  ShapeHandler();
  ~ShapeHandler();

  void onPaint(QPainter *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  std::vector<Shape*> shapes_;

  Shape *activated_shape_;
};
