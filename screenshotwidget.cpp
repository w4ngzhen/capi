#include "screenshotwidget.h"
#include "./ui_screenshotwidget.h"

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

