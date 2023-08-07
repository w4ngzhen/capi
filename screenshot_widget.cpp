#include "screenshot_widget.h"
#include "./ui_screenshot_widget.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>

#include "helper/math_helper.h"
#include "helper/paint_helper.h"
#include "screenshot_status.h"

ScreenShotWidget::ScreenShotWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenShotWidget),
      status_(ScreenShotStatus::Explore) {
  ui->setupUi(this);

  // QT中 MouseMoveEvent为了降低计算资源，默认需要要鼠标按下才能触发该事件。
  // 要想鼠标不按下时的移动也能捕捉到，需要setMouseTracking(true)
  setMouseTracking(true);

  // 获取当前主屏幕
  QScreen *screen = QGuiApplication::primaryScreen();

  // 主屏幕的size
  this->screen_size_ = screen->geometry().size();
  // 获取当前主屏幕的图片（注意，这是真实像素的图片，在高分屏上，要注意鼠标坐标）
  this->screen_pic_ = screen->grabWindow(0).toImage();
  const auto screenPicSize = this->screen_pic_.size();
  this->screen_scale_ =
      QSize(screenPicSize.width() / this->screen_size_.width(),
            screenPicSize.height() / this->screen_size_.height());

  // 初始状态
  this->status_ = ScreenShotStatus::Explore;

  // 初始化各个层
  QImage *pScreenPic = &this->screen_pic_;
  this->explore_layer_ =
      new ExploreLayer(pScreenPic, this->screen_size_, this->screen_scale_);
  this->capturing_layer_ = new CapturingLayer(pScreenPic);
  this->captured_layer_ =
      new CapturedLayer(pScreenPic, this->screen_size_, this->screen_scale_);

  // 信号连接
  connect(this->capturing_layer_, &CapturingLayer::capturingFinishedSignal,
          this, &ScreenShotWidget::handleCapturingFinished);
  connect(this->captured_layer_, &CapturedLayer::saveCapturedRectSignal, this,
          &ScreenShotWidget::handleCapturedRect);

  // 无边框显示
  this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
}

ScreenShotWidget::~ScreenShotWidget() {
  delete ui;
  delete this->explore_layer_;
  delete this->capturing_layer_;
  delete this->captured_layer_;
}

/**
 * @brief ScreenShotWidget::handleCapturingFinished
 * captruingLayer处理完成后，触发该处进行截取的处理
 * @param sizeValid
 * @param capturedRect
 */
void ScreenShotWidget::handleCapturingFinished(bool sizeValid,
                                               QRect *capturedRect) {
  if (!sizeValid) {
    this->status_ = ScreenShotStatus::Explore;
    this->captured_layer_->setCapturedRect(QRect());
  } else {
    this->status_ = ScreenShotStatus::Captured;
    QRect rect = QRect(capturedRect->x(), capturedRect->y(),
                       capturedRect->width(), capturedRect->height());
    this->captured_layer_->setCapturedRect(rect);
  }

  this->update();
}

/**
 * @brief ScreenShotWidget::handleCapturedRect
 * 处理CapturedLayer发射出的保存截屏的指定
 */
void ScreenShotWidget::handleCapturedRect(QRect logicRect, QRect realRect,
                                          CapturedRectSaveType saveType) {
  const QImage pic = this->screen_pic_.copy(
      realRect.x(), realRect.y(), realRect.width(), realRect.height());
  if (saveType == CapturedRectSaveType::ToClipboard) {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage(pic);
  }
  // 保存到目标位置后，退出截图应用
  this->close();
}

void ScreenShotWidget::paintEvent(QPaintEvent *) {

  // 构造Painter
  QPainter painter(this);
  //    painter.setRenderHint(QPainter::Antialiasing, true);

  // 将截屏图像绘制在整个窗体
  painter.drawImage(QRect(0, 0, this->width(), this->height()),
                    this->screen_pic_);

  switch (this->status_) {
  case ScreenShotStatus::Explore:
    this->setCursor(QCursor(Qt::CrossCursor));
    this->explore_layer_->paint(painter);
    break;
  case ScreenShotStatus::Capturing:
    this->setCursor(QCursor(Qt::CrossCursor));
    this->capturing_layer_->paint(painter);
    break;
  case ScreenShotStatus::Captured:
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->captured_layer_->paint(painter);
    break;
  default:
    break;
  }
}

void ScreenShotWidget::mousePressEvent(QMouseEvent *event) {

  switch (this->status_) {
  case ScreenShotStatus::Explore:
    // explore态鼠标点击，设置capturing层起始点
    this->capturing_layer_->setStartPos(event->pos());
    // 切换状态
    this->status_ = ScreenShotStatus::Capturing;
    break;
  case ScreenShotStatus::Captured:
    this->captured_layer_->mousePressEvent(event);
    break;
  default:
    break;
  }
  this->update();
}

void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *event) {

  switch (this->status_) {
  case ScreenShotStatus::Capturing:
    this->captured_layer_->resetStatus();
    this->capturing_layer_->mouseReleaseEvent(event);
    this->status_ = ScreenShotStatus::Captured;
    break;
  case ScreenShotStatus::Captured:
    this->captured_layer_->mouseReleaseEvent();
    break;
  default:
    break;
  }

  this->update();
}

void ScreenShotWidget::mouseMoveEvent(QMouseEvent *event) {
  // 鼠标位置变化需要通知给所有的layer
  this->explore_layer_->mouseMoveEvent(event);
  this->capturing_layer_->mouseMoveEvent(event);
  this->captured_layer_->mouseMoveEvent(event);
  this->update();
}

void ScreenShotWidget::keyReleaseEvent(QKeyEvent *event) {
  int key = event->key();
  if (key == Qt::Key_Escape) {
    // ESC键处理
    if (this->status_ == ScreenShotStatus::Captured ||
        this->status_ == ScreenShotStatus::Capturing) {
      this->status_ = ScreenShotStatus::Explore;
      this->update();
      return;
    }

    if (this->status_ == ScreenShotStatus::Explore) {
      // ESC退出截图，则需要清理粘贴板的图片数据
      QClipboard *clipboard = QGuiApplication::clipboard();
      clipboard->clear();
      this->close();
    }
    return;
  }
  if (key == Qt::Key_Right || key == Qt::Key_Left || key == Qt::Key_Up ||
      key == Qt::Key_Down) {
    this->explore_layer_->keyMoveEvent(key);
    this->update();
  }
}

void ScreenShotWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  this->captured_layer_->mouseDoubleClickEvent(event);
}
