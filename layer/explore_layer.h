#ifndef EXPLORE_LAYER_H
#define EXPLORE_LAYER_H

#include <QPoint>

class QPainter;
class QMouseEvent;
class QImage;

class ExploreLayer
{
public:
    ExploreLayer(QImage *);
    ~ExploreLayer();
private:
    QImage *screen_pic_;
    QPoint mouse_pos_;

public:
    void paint(QPainter &painter);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // EXPLORE_LAYER_H
