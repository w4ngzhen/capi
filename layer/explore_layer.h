#ifndef EXPLORE_LAYER_H
#define EXPLORE_LAYER_H

#include <QPoint>
#include <QSize>

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
    QSize assistant_panel_size_;

public:
    void paint(QPainter &painter);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void paintAssistantPanel(QPainter &painter);
    void paintAssistantPanelAt(int panelX, int panelY, QPainter &painter);
};

#endif // EXPLORE_LAYER_H
