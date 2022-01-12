#include "screenshotwidget.h"
#include "./ui_screenshotwidget.h"

#include <QPainter>

ScreenShotWidget::ScreenShotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScreenShotWidget)
{
    ui->setupUi(this);
}

ScreenShotWidget::~ScreenShotWidget()
{
    delete ui;
}

void ScreenShotWidget::paintEvent(QPaintEvent *)
{

    // 创建一个路径Path，并指定路径范围是整个窗体的内边距10px
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);

    // 构造Painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 设置用(0, 111, 222)颜色进行填充
    painter.fillPath(path, QBrush(QColor(0, 111, 222)));
}
