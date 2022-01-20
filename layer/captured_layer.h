#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QRect>
#include <QSize>
class QPainter;

class CapturedLayer
{
public:
    CapturedLayer(QSize screenSize);
    void setCapturedRect(QRect &rect);
    void paint(QPainter &painter);
    ~CapturedLayer(){}
private:
    QSize screen_size_;
    QRect captured_rect_;
};

#endif // CAPTURED_LAYER_H
