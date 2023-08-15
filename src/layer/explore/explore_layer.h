#ifndef EXPLORE_LAYER_H
#define EXPLORE_LAYER_H

#include <QObject>
#include <QPoint>
#include <QSize>

class QPainter;
class QMouseEvent;
class QImage;
class QKeyEvent;

class ExploreLayer : public QObject {
  // 会用到QT的signal/slot机制
  Q_OBJECT
public:
  ExploreLayer(QImage *, QSize canvasSize);
  void setCanvasSize(QSize);
  ~ExploreLayer();

private:
  QImage *canvas_img_;
  QSize canvas_size_;
  QPoint mouse_pos_;

public:
  void paint(QPainter &painter);
  void mouseMoveEvent(QMouseEvent *event);
  void keyReleaseEvent(QKeyEvent *);

signals:
  void signalQuitCurrentLayer();

private:
  void paintToolPane(QPainter &painter);
  void paintToolPaneAt(int panelX, int panelY, QPainter &painter);
};

#endif // EXPLORE_LAYER_H
