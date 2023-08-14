#ifndef CANVAS_WIDGET_H
#define CANVAS_WIDGET_H

#include <QImage>
#include <QSize>
#include <QWidget>

#include "layer/captured_layer.h"
#include "layer/capturing_layer.h"
#include "layer/explore_layer.h"

#include "canvas_status.h"
#include "helper/math_helper.h"

class CanvasWidget : public QWidget {
  Q_OBJECT

public:
  CanvasWidget(QWidget *parent = nullptr);
  void init(QImage *);
  ~CanvasWidget();

private slots:
  void handleSignalCapturingFinished(bool, QRect *);
  void handleSignalCapturedRect(QRect capturedRect, CapturedRectSaveType);
  void handleSignalQuitLayer(CanvasStatus);

private:
  // 原始分辨率的图片
  QImage *canvas_img_;

  CanvasStatus status_;
  // 鼠标在捕获截取操作前的移动探索
  ExploreLayer *explore_layer_;
  // 鼠标按下后捕获过程
  CapturingLayer *capturing_layer_;
  // 完成截图区域捕获后的处理
  CapturedLayer *captured_layer_;

  // QWidget interface
protected:
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
  virtual void paintEvent(QPaintEvent *) override;
  virtual void resizeEvent(QResizeEvent *) override;
};

#endif // CANVAS_WIDGET_H
