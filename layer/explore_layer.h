#ifndef EXPLORE_LAYER_H
#define EXPLORE_LAYER_H

#include <QPoint>
#include <QSize>

class QPainter;
class QMouseEvent;

class ExploreLayer
{
public:
    ExploreLayer(QSize);
    ~ExploreLayer();
private:
    QSize screen_size_;
    QPoint mouse_pos_;

public:
    void paint(QPainter &painter);
    void setScreenSize(const QSize &size);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // EXPLORE_LAYER_H
