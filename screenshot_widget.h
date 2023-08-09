#ifndef SCREENSHOT_WIDGET_H
#define SCREENSHOT_WIDGET_H

#include <QImage>
#include <QSize>
#include <QWidget>

#include "layer/captured_layer.h"
#include "layer/capturing_layer.h"
#include "layer/explore_layer.h"

#include "helper/math_helper.h"
#include "screenshot_status.h"

class ScreenShotWidget : public QWidget {
  Q_OBJECT

public:
  ScreenShotWidget(QWidget *parent = nullptr);
  ~ScreenShotWidget();

protected:
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void resizeEvent(QResizeEvent *event) override;
private slots:
  void handleCapturingFinished(bool, QRect *);
  void handleCapturedRect(QRect capturedRect, CapturedRectSaveType);

private:
  // 屏幕截图，原始分辨率
  QImage screen_pic_;

  ScreenShotStatus status_;
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
};

#endif // SCREENSHOT_WIDGET_H
