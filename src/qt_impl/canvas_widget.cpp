#include <QClipboard>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include "core/canvas.h"

#include "canvas_widget.h"
#include "core/event/captured_image_save_event.h"
#include "core/global/global.h"
#include "qt_impl/image_qt_impl.h"
#include "qt_impl/painter_qt_impl.h"

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), canvas_(nullptr) {
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
  this->canvas_->setCanvasEventOnQuitCb([this] { handleCanvasEventOnQuit(); });
  this->canvas_->setCanvasEventOnImageSaveCb([this](auto event) {
    handleCanvasEventOnImageSave(event);
  });
}

void CanvasWidget::handleCanvasEventOnQuit() {
  // ESC退出截图，则需要清理粘贴板的图片数据
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->clear();
  this->close();
}

void CanvasWidget::handleCanvasEventOnImageSave(const capi::CapturedImageSaveEvent *event) {
  auto capturedRect = event->captured_rect();
  // 明确知道是ImageQtImpl指针
  auto implRealImg = (ImageQtImpl *) this->canvas_->canvas_image();
  // 获取image的实际像素尺寸，在高分屏上可能和屏幕逻辑像素不一致。
  auto picRealSize = implRealImg->size();
  // 画布尺寸，这个尺寸一般和屏幕逻辑尺寸一致。
  auto canvasSize = this->canvas_->size();
  // 等比计算得到逻辑的矩形rect应该在实际像素的图像上的rect
  int realRectX = capturedRect.x() * picRealSize.w() / canvasSize.w();
  int realRectY = capturedRect.y() * picRealSize.h() / canvasSize.h();
  int realRectW =
      capturedRect.w() * picRealSize.w() / canvasSize.w();
  int realRectH =
      capturedRect.h() * picRealSize.h() / canvasSize.h();
  const QImage pic =
      implRealImg->getQImage()->copy(realRectX, realRectY, realRectW, realRectH);
  if (event->save_mode() == SaveMode::Clipboard) {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage(pic);
  }
  // 保存到目标位置后，退出截图应用
  this->close();

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
  this->canvas_->onKeyPress((capi::Key) key,
                            (capi::KeyboardModifier) keyModifiers.toInt());
  this->update();
}

void CanvasWidget::mouseDoubleClickEvent(QMouseEvent *event) {
  auto pos = event->pos();
  this->canvas_->onMouseDoubleClick(Point(pos.x(), pos.y()));
}

void CanvasWidget::resizeEvent(QResizeEvent *event) {
  auto size = event->size();
  this->canvas_->onResize(Size(size.width(), size.height()));
}
