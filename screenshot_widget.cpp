#include "screenshot_widget.h"
#include "./ui_screenshot_widget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScreen>

#include "layer/explore_layer.h"
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
    this->explore_layer_ = new ExploreLayer(this->size());
    this->capturing_layer_ = new CapturingLayer(this->size());
}

ScreenShotWidget::~ScreenShotWidget()
{
    delete ui;
    delete this->explore_layer_;
    delete this->capturing_layer_;
}

void ScreenShotWidget::paintCapturedRect(QPainter &painter)
{
    // 将截图矩形的周围都添加灰色蒙版
    int rX = this->captured_rect_.x();
    int rY = this->captured_rect_.y();
    int rW = this->captured_rect_.width();
    int rH = this->captured_rect_.height();
    // 得到捕获区域左侧的矩形
    QRect leftMaskRect(0, 0, rX, this->height());
    QRect rightMaskRect(rX + rW, 0, this->width() - (rX + rW), this->height());
    QRect topMaskRect(rX, 0, rW, rY);
    QRect bottomMaskRect(rX, (rY + rH), rW, this->height() - (rY + rH));

    QBrush grayBrush = QBrush(QColor(0, 0, 0, 50)); // 50% Alpha的灰色
    painter.fillRect(leftMaskRect, grayBrush);
    painter.fillRect(rightMaskRect, grayBrush);
    painter.fillRect(topMaskRect, grayBrush);
    painter.fillRect(bottomMaskRect, grayBrush);

    // 增加一个边框
    painter.setPen(QPen(QColor(0, 111, 222), 2));
    painter.drawRect(this->captured_rect_);
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
        this->paintCapturedRect(painter);
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
    this->mouse_pos_ = event->pos();

    this->explore_layer_->mouseMoveEvent(event);
    this->capturing_layer_->mouseMoveEvent(event);

    this->update();
}

void ScreenShotWidget::resizeEvent(QResizeEvent *event)
{
    this->explore_layer_->setScreenSize(event->size());
    this->capturing_layer_->setScreenSize(event->size());
}
