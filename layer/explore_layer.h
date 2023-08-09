#ifndef EXPLORE_LAYER_H
#define EXPLORE_LAYER_H

#include <QPoint>
#include <QSize>

class QPainter;
class QMouseEvent;
class QImage;

class ExploreLayer {
public:
  ExploreLayer(QImage *, QSize canvasSize);
  void setCanvasSize(QSize);
  ~ExploreLayer();

private:
  QImage *screen_pic_;
  QSize canvas_size_;
  QPoint mouse_pos_;

public:
  void paint(QPainter &painter);
  void mouseMoveEvent(QMouseEvent *event);
  /*
   * 在exploring阶段，允许用户按上下左右方向键操控鼠标
   * */
  void keyMoveEvent(const int key);

private:
  void paintToolPane(QPainter &painter);
  void paintToolPaneAt(int panelX, int panelY, QPainter &painter);
};

#endif // EXPLORE_LAYER_H
