#ifndef CANVAS_WIDGET_H
#define CANVAS_WIDGET_H

#include <QImage>
#include <QSize>
#include <QWidget>

#include "core/canvas.h"

class CanvasWidget : public QWidget {
  Q_OBJECT

public:
  explicit CanvasWidget(QWidget *parent = nullptr);
  void init(QImage *);
  ~CanvasWidget() override;

private:
  capi::Canvas *canvas_{};

  void handleOnCanvasQuitCb();
  // QWidget interface
protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
};

#endif // CANVAS_WIDGET_H
