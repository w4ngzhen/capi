#ifndef CAPTURING_LAYER_H
#define CAPTURING_LAYER_H

#include <QSize>
#include <QObject>
#include <QPainter>
#include <QMouseEvent>

class QImage;


class CapturingLayer: public QObject
{
    // 会用到QT的signal/slot机制
    Q_OBJECT
public:
    CapturingLayer(QImage*);
    void paint(QPainter &painter);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setStartPos(QPoint);
    ~CapturingLayer();
signals:
    void capturingFinishedSignal(bool sizeValid, QRect *capturedRect);
private:
    // 屏幕图像
    QImage *screen_pic_;
    // 记录随时的鼠标位置
    QPoint mouse_pos_;
    // 记录鼠标按下的位置
    QPoint start_pos_;
};

#endif // CAPTURING_LAYER_H
