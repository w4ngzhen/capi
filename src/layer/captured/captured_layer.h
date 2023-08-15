#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QMouseEvent>
#include <QRect>
#include <QSize>

class QPainter;
class QImage;
class QKeyEvent;

enum CapturedRectSaveType { ToClipboard, ToFile };

enum OperationMode {
  Normal = 0,

  DraggingLeftTop = 0x01,
  DraggingRightTop = 0x02,
  DraggingLeftBottom = 0x03,
  DraggingRightBottom = 0x04,
  DraggingArea = 0x10,

  EnableDrawRect = 0x20,
  DrawingRect = 0x21,
};

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

  QPoint mouse_last_pos_;

  OperationMode opr_mode_;
};

#endif // CAPTURED_LAYER_H
