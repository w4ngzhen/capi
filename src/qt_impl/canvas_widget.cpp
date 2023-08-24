#include <QClipboard>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include "core/canvas.h"

#include "canvas_widget.h"
#include "core/global/global.h"
#include "qt_impl/image_qt_impl.h"
#include "qt_impl/painter_qt_impl.h"

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent), canvas_(nullptr) {
  // QT中 MouseMoveEvent为了降低计算资源，默认需要要鼠标按下才能触发该事件。
  // 要想鼠标不按下时的移动也能捕捉到，需要setMouseTracking(true)
  setMouseTracking(true);
  // 无边框显示
  //  this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
}

void CanvasWidget::init(QImage *img) {
  auto *pImgQtImpl = new ImageQtImpl(img);
  // pImgQtImpl指针在canvas析构时会被释放
  this->canvas_ = new capi::Canvas(pImgQtImpl);
}

CanvasWidget::~CanvasWidget() { delete this->canvas_; }

void CanvasWidget::paintEvent(QPaintEvent *) {
  // 构造Painter
  QPainter qPainter(this);
  qPainter.setRenderHint(QPainter::Antialiasing, true);

  Painter *painter = new PainterQtImpl(&qPainter);
  this->canvas_->onPaint(painter);
  delete painter;
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
  auto pos = event->pos();
  this->canvas_->onMousePress(Point(pos.x(), pos.y()));
  this->update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
  auto pos = event->pos();
  this->canvas_->onMouseRelease(Point(pos.x(), pos.y()));
  this->update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
  auto pos = event->pos();
  this->canvas_->onMouseMove(Point(pos.x(), pos.y()));
  this->update();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent *event) { this->update(); }

void CanvasWidget::keyPressEvent(QKeyEvent *event) { 
  auto key = event->key();
  auto keyModifiers = event->modifiers();
  this->canvas_->onKeyPress((capi::Key)key, (capi::KeyboardModifier)keyModifiers.toInt());
  this->update();
}

void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event) {}

void CanvasWidget::resizeEvent(QResizeEvent *event) {
  auto size = event->size();
  this->canvas_->onResize(Size(size.width(), size.height()));
}
