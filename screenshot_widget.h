#ifndef SCREENSHOT_WIDGET_H
#define SCREENSHOT_WIDGET_H

#include <QWidget>

#include "layer/explore_layer.h"
#include "layer/capturing_layer.h"
#include "helper/math_helper.h"
#include "screenshot_status.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ScreenShotWidget; }
QT_END_NAMESPACE

class ScreenShotWidget : public QWidget
{
    Q_OBJECT

public:
    ScreenShotWidget(QWidget *parent = nullptr);
    ~ScreenShotWidget();
private:
    void paintCapturingRect(QPainter &);
    void paintCapturedRect(QPainter &);
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void handleCapturingFinished(bool, QRect*);

private:
    Ui::ScreenShotWidget *ui;
    QPixmap screen_pic_;
    QPoint mouse_pos_;
    QPoint mouse_down_pos_;
    // 当状态为捕获完成以后，对应的桌面的捕获区域（截取的区域）
    QRect captured_rect_;

    ScreenShotStatus status_;
    // 鼠标在捕获截取操作前的移动探索
    ExploreLayer *explore_layer_;
    CapturingLayer *capturing_layer_;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

};
#endif // SCREENSHOT_WIDGET_H
