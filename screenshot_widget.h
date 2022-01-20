#ifndef SCREENSHOT_WIDGET_H
#define SCREENSHOT_WIDGET_H

#include <QWidget>
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
    void paintMousePositionLine(QPainter &);
    void paintCapturingRect(QPainter &);
    void paintCapturedRect(QPainter &);
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::ScreenShotWidget *ui;
    QPixmap screen_pic_;
    QPoint mouse_pos_;
    QPoint mouse_down_pos_;
    // 当状态为捕获完成以后，对应的桌面的捕获区域（截取的区域）
    QRect captured_rect_;

    ScreenShotStatus status_;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};
#endif // SCREENSHOT_WIDGET_H
