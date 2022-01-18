#include "screenshotwidget.h"
#include "./ui_screenshotwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include "screenshotutils.h"


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
}

ScreenShotWidget::~ScreenShotWidget()
{
    delete ui;
}

void ScreenShotWidget::paintMousePositionLine(QPainter &painter)
{
    // 绘制鼠标所在的位置垂直和水平线
    painter.setPen(QPen(Qt::red, 1));
    int mX = this->mouse_pos_.x();
    int mY = this->mouse_pos_.y();
    // 垂直线
    painter.drawLine(mX, 0, mX, this->height());
    // 水平线
    painter.drawLine(0, mY, this->width(), mY);
}

void ScreenShotWidget::paintCapturingRect(QPainter &painter)
{
    painter.setPen(QPen(QColor(0, 111, 222), 2));
    int mX = this->mouse_pos_.x();
    int mY = this->mouse_pos_.y();
    int dX = this->mouse_down_pos_.x();
    int dY = this->mouse_down_pos_.y();
    QRect rect = ScreenShotUtils::calcRect(mX, mY, dX, dY);
    painter.drawRect(rect);
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
        this->paintMousePositionLine(painter);
    }
    else if (this->status_ == ScreenShotStatus::Capturing)
    {
        this->setCursor(QCursor(Qt::CrossCursor));
        this->paintCapturingRect(painter);
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
    this->mouse_down_pos_ = event->pos();
    this->update();
}

void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // 根据鼠标按下的位置和此时抬起的位置，计算捕获的矩形
    QPoint &downPos = this->mouse_down_pos_;
    QPoint currentPos = event->pos();
    QRect capturedRect = ScreenShotUtils::calcRect(
                currentPos.x(), currentPos.y(), downPos.x(), downPos.y());

    if (capturedRect.width() <= 3 || capturedRect.height() <= 3)
    {
        // 截图区域过于小，不予捕获，回到Explore状态
        this->status_ = ScreenShotStatus::Explore;
        this->captured_rect_ = QRect();
    }
    else
    {
        this->status_ = ScreenShotStatus::Captured;
        this->captured_rect_ = capturedRect;
    }

    this->update();
}

void ScreenShotWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->mouse_pos_ = event->pos();
    // 调用update，触发QT的绘制事件
    this->update();
}
