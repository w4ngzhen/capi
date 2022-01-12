#include "screenshotwidget.h"
#include "./ui_screenshotwidget.h"

#include <QPainter>
#include <QScreen>

ScreenShotWidget::ScreenShotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScreenShotWidget)
{
    ui->setupUi(this);

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
}
