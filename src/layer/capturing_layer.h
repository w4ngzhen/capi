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
    void paint(QPainter &);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void setStartPos(QPoint);
    ~CapturingLayer();

signals:
    void signalCapturingFinished(bool sizeValid, QRect *capturedRect);
    void signalQuitCurrentLayer();

private:
    // 画布图像
    QImage *canvas_img_;
    // 记录随时的鼠标位置
    QPoint mouse_pos_;
    // 记录鼠标按下的位置
    QPoint start_pos_;
};

#endif // CAPTURING_LAYER_H
