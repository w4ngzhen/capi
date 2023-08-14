#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QMouseEvent>
#include <QRect>
#include <QSize>

class QPainter;
class QImage;
class QKeyEvent;

enum CapturedRectSaveType { ToClipboard, ToFile };

class CapturedLayer : public QObject {
  Q_OBJECT
public:
  CapturedLayer(QImage *, QSize canvasSize);
  void setCapturedRect(const QRect &rect);
  void setCanvasSize(QSize);
  void paint(QPainter &painter);
  void mouseDoubleClickEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent();
  void keyReleaseEvent(QKeyEvent *);
  void resetStatus();
  ~CapturedLayer() {}
signals:
  // 信号：完成当前捕获
  void signalSaveCapturedRect(QRect capruredRect,
                              CapturedRectSaveType saveType);
  // 信号：退出当前状态
  void signalQuitCurrentLayer();

private:
  QImage *canvas_img_;
  QSize canvas_size_;
  QRect captured_rect_;
  // 是否整体区域拖动
  bool is_area_dragging_;
  /*
   * 四个角的拖动
   * 0: lt, 1: rt, 2: lb, 3: rb
   * 小于0:nothing
   * */
  int dragging_corner_;

  QPoint mouse_last_pos_;
};

#endif // CAPTURED_LAYER_H
