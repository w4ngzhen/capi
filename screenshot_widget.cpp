#include "screenshot_widget.h"
#include "./ui_screenshot_widget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScreen>

#include "helper/math_helper.h"
#include "helper/paint_helper.h"

ScreenShotWidget::ScreenShotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScreenShotWidget)
    , status_(ScreenShotStatus::Explore)
{
    ui->setupUi(this);

    // QT中 MouseMoveEvent为了降低计算资源，默认需要要鼠标按下才能触发该事件。
    // 要想鼠标不按下时的移动也能捕捉到，需要setMouseTracking(true)
    setMouseTracking(true);

    // 获取当前屏幕图像
    QScreen *screen = QGuiApplication::primaryScreen();
    this->screen_pic_ = screen->grabWindow(0);

    // 初始状态
    this->status_ = ScreenShotStatus::Explore;

    // 初始化各个层
    this->explore_layer_ = new ExploreLayer(this->size());
    this->capturing_layer_ = new CapturingLayer(this->size());
    this->captured_layer_ = new CapturedLayer(this->size());

    // 信号连接
    connect(this->capturing_layer_,
            &CapturingLayer::capturingFinishedSignal,
            this,
            &ScreenShotWidget::handleCapturingFinished);
}

ScreenShotWidget::~ScreenShotWidget()
{
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
void ScreenShotWidget::handleCapturingFinished(
        bool sizeValid,
        QRect *capturedRect)
{
    if(!sizeValid)
    {
        this->status_ = ScreenShotStatus::Explore;
        this->captured_layer_->setCapturedRect(QRect());
    }
    else
    {
        this->status_ = ScreenShotStatus::Captured;
        QRect rect = QRect(capturedRect->x(),
                           capturedRect->y(),
                           capturedRect->width(),
                           capturedRect->height());
        this->captured_layer_->setCapturedRect(rect);
    }
}

void ScreenShotWidget::paintEvent(QPaintEvent *)
{

    // 构造Painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 将截屏图像绘制在整个窗体
    painter.drawPixmap(0, 0, this->width(), this->height(), this->screen_pic_);

    if (this->status_ == ScreenShotStatus::Explore)
    {
        this->setCursor(QCursor(Qt::CrossCursor));
        this->explore_layer_->paint(painter);
    }
    else if (this->status_ == ScreenShotStatus::Capturing)
    {
        this->setCursor(QCursor(Qt::CrossCursor));
        this->capturing_layer_->paint(painter);
    }
    else if (this->status_ == ScreenShotStatus::Captured)
    {
        this->setCursor(QCursor(Qt::ArrowCursor));
        this->captured_layer_->paint(painter);
    }
}

void ScreenShotWidget::mousePressEvent(QMouseEvent *event)
{
    this->status_ = ScreenShotStatus::Capturing;
    this->capturing_layer_->mousePressEvent(event);

    this->update();
}

void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->capturing_layer_->mouseReleaseEvent(event);

    this->update();
}

void ScreenShotWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->explore_layer_->mouseMoveEvent(event);
    this->capturing_layer_->mouseMoveEvent(event);

    this->update();
}

void ScreenShotWidget::resizeEvent(QResizeEvent *event)
{
    this->explore_layer_->setScreenSize(event->size());
    this->capturing_layer_->setScreenSize(event->size());
    this->captured_layer_->setScreenSize(event->size());
}