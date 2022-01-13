#include "screenshotwidget.h"
#include "./ui_screenshotwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QScreen>



ScreenShotWidget::ScreenShotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScreenShotWidget)
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

void ScreenShotWidget::paintEvent(QPaintEvent *)
{

    // 构造Painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 将截屏图像绘制在整个窗体
    painter.drawPixmap(0, 0, this->width(), this->height(), this->screen_pic_);

    // 绘制鼠标所在的位置垂直和水平线
    painter.setPen(QPen(Qt::red, 1));
    int mX = this->mouse_pos_.x();
    int mY = this->mouse_pos_.y();
    // 垂直线
    painter.drawLine(mX, 0, mX, this->height());
    // 水平线
    painter.drawLine(0, mY, this->width(), mY);

}


void ScreenShotWidget::mousePressEvent(QMouseEvent *event)
{

}

void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void ScreenShotWidget::mouseMoveEvent(QMouseEvent *event)
{
    this->mouse_pos_ = event->pos();
    // 调用update，触发QT的绘制事件
    this->update();
}
