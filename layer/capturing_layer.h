#ifndef CAPTURING_LAYER_H
#define CAPTURING_LAYER_H

#include <QSize>
#include <QObject>
#include <QPainter>
#include <QMouseEvent>


class CapturingLayer: public QObject
{
    // 会用到QT的signal/slot机制
    Q_OBJECT
public:
    CapturingLayer(QSize screenSize);
    void paint(QPainter &painter);
    void setScreenSize(const QSize &size);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    ~CapturingLayer();
signals:
    void capturingFinishedSignal(bool sizeValid, QRect *capturedRect);
private:
    // 记录屏幕尺寸
    QSize screen_size_;
    // 记录随时的鼠标位置
    QPoint mouse_pos_;
    // 记录鼠标按下的位置
    QPoint mouse_down_pos_;
};

#endif // CAPTURING_LAYER_H
