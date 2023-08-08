#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QMouseEvent>
#include <QRect>
#include <QSize>
class QPainter;
class QImage;

enum CapturedRectSaveType { ToClipboard, ToFile };

class CapturedLayer : public QObject {
  Q_OBJECT
public:
  CapturedLayer(QImage *, QSize screenSize, QSize screenScale);
  void setCapturedRect(const QRect &rect);
  void paint(QPainter &painter);
  void mouseDoubleClickEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent();
  void resetStatus();
  ~CapturedLayer() {}
signals:
  void saveCapturedRectSignal(QRect logicRect, QRect realRect,
                              CapturedRectSaveType saveType);

private:
  QImage *screen_pic_;
  QSize screen_scale_;
  QSize screen_size_;
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
