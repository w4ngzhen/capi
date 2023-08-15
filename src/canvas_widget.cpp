#include <QClipboard>
#include <QDebug>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include "helper/math_helper.h"
#include "helper/paint_helper.h"

#include "canvas_status.h"
#include "canvas_widget.h"

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), status_(CanvasStatus::Explore) {
  // QT中 MouseMoveEvent为了降低计算资源，默认需要要鼠标按下才能触发该事件。
  // 要想鼠标不按下时的移动也能捕捉到，需要setMouseTracking(true)
  setMouseTracking(true);
  // 无边框显示
  //  this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
}

void CanvasWidget::init(QImage *img) {
  this->canvas_img_ = img;
  const auto canvasSize = this->canvas_img_->size();
  //
  // 初始状态
  this->status_ = CanvasStatus::Explore;

  QSize initSize(100, 100);
  this->explore_layer_ = new ExploreLayer(this->canvas_img_, initSize);
  this->capturing_layer_ = new CapturingLayer(this->canvas_img_);
  this->captured_layer_ = new CapturedLayer(this->canvas_img_, initSize);

  // 信号连接
  connect(this->capturing_layer_, &CapturingLayer::signalCapturingFinished,
          this, &CanvasWidget::handleSignalCapturingFinished);

  // 处理captured layer触发的保存事件
  connect(this->captured_layer_, &CapturedLayer::signalSaveCapturedRect, this,
          &CanvasWidget::handleSignalCapturedRect);

  // 处理三个layer的quit事件
  connect(this->explore_layer_, &ExploreLayer::signalQuitCurrentLayer, this,
          [&]() { this->handleSignalQuitLayer(CanvasStatus::Explore); });
  connect(this->capturing_layer_, &CapturingLayer::signalQuitCurrentLayer, this,
          [&]() { this->handleSignalQuitLayer(CanvasStatus::Capturing); });
  connect(this->captured_layer_, &CapturedLayer::signalQuitCurrentLayer, this,
          [&]() { this->handleSignalQuitLayer(CanvasStatus::Captured); });
}

CanvasWidget::~CanvasWidget() {
  delete this->explore_layer_;
  delete this->capturing_layer_;
  delete this->captured_layer_;
}

/**
 * captruingLayer处理完成后，触发该处进行截取的处理
 * @param sizeValid
 * @param capturedRect
 */
void CanvasWidget::handleSignalCapturingFinished(bool sizeValid,
                                                 QRect *capturedRect) {
  if (!sizeValid) {
    this->status_ = CanvasStatus::Explore;
    this->captured_layer_->setCapturedRect(QRect());
  } else {
    this->status_ = CanvasStatus::Captured;
    QRect rect = QRect(capturedRect->x(), capturedRect->y(),
                       capturedRect->width(), capturedRect->height());
    this->captured_layer_->setCapturedRect(rect);
  }

  this->update();
}

/**
 * 处理CapturedLayer发射出的保存截屏的指定
 * 注意，信号发出的捕获的区域仅仅是逻辑区域，需要转化为图片上的实际像素区域
 */
void CanvasWidget::handleSignalCapturedRect(QRect capturedRect,
                                            CapturedRectSaveType saveType) {
  auto picRealSize = this->canvas_img_->size();
  auto canvasSize = this->size();
  int realRectX = capturedRect.x() * picRealSize.width() / canvasSize.width();
  int realRectY = capturedRect.y() * picRealSize.height() / canvasSize.height();
  int realRectW =
      capturedRect.width() * picRealSize.width() / canvasSize.width();
  int realRectH =
      capturedRect.height() * picRealSize.height() / canvasSize.height();
  const QImage pic =
      this->canvas_img_->copy(realRectX, realRectY, realRectW, realRectH);
  if (saveType == CapturedRectSaveType::ToClipboard) {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage(pic);
  }
  // 保存到目标位置后，退出截图应用
  this->close();
}

void CanvasWidget::handleSignalQuitLayer(CanvasStatus status) {

  if (status == CanvasStatus::Captured || status == CanvasStatus::Capturing) {
    // 更新状态
    this->status_ = CanvasStatus::Explore;
    this->update();
    return;
  }

  if (status == CanvasStatus::Explore) {
    // ESC退出截图，则需要清理粘贴板的图片数据
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->clear();
    this->close();
  }
  return;
}

void CanvasWidget::paintEvent(QPaintEvent *) {

  // 构造Painter
  QPainter painter(this);
  //    painter.setRenderHint(QPainter::Antialiasing, true);

  // 将图像绘制在整个窗体
  painter.drawImage(QRect(0, 0, this->width(), this->height()),
                    *this->canvas_img_);

  switch (this->status_) {
  case CanvasStatus::Explore:
    this->setCursor(QCursor(Qt::BlankCursor));
    this->explore_layer_->paint(painter);
    break;
  case CanvasStatus::Capturing:
    this->setCursor(QCursor(Qt::CrossCursor));
    this->capturing_layer_->paint(painter);
    break;
  case CanvasStatus::Captured:
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->captured_layer_->paint(painter);
    break;
  default:
    break;
  }
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {

  switch (this->status_) {
  case CanvasStatus::Explore:
    // explore态鼠标点击，设置capturing层起始点
    this->capturing_layer_->setStartPos(event->pos());
    // 切换状态
    this->status_ = CanvasStatus::Capturing;
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->mousePressEvent(event);
    break;
  default:
    break;
  }
  this->update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {

  switch (this->status_) {
  case CanvasStatus::Capturing:
    this->captured_layer_->resetStatus();
    this->capturing_layer_->mouseReleaseEvent(event);
    this->status_ = CanvasStatus::Captured;
    break;
  case CanvasStatus::Captured:
    this->captured_layer_->mouseReleaseEvent();
    break;
  default:
    break;
  }

  this->update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
  // 鼠标位置变化需要通知给所有的layer
  this->explore_layer_->mouseMoveEvent(event);
  this->capturing_layer_->mouseMoveEvent(event);
  this->captured_layer_->mouseMoveEvent(event);
  this->update();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent *event) {

  switch (this->status_) {
  case CanvasStatus::Explore:
    this->explore_layer_->keyReleaseEvent(event);
  case CanvasStatus::Capturing:
    this->capturing_layer_->keyReleaseEvent(event);
  case CanvasStatus::Captured:
    this->captured_layer_->keyReleaseEvent(event);
  default:
    break;
  }
  this->update();
}

void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  this->captured_layer_->mouseDoubleClickEvent(event);
}
/**
 * 重要：需要在resizeEvent中，更新canvas size
 **/
void CanvasWidget::resizeEvent(QResizeEvent *event) {
  auto canvasSize = event->size();
  this->explore_layer_->setCanvasSize(canvasSize);
  this->captured_layer_->setCanvasSize(canvasSize);
}
