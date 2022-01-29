#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QMouseEvent>
#include <QRect>
#include <QSize>
class QPainter;
class QImage;

enum CapturedRectSaveType
{
    ToClipboard,
    ToFile
};

class CapturedLayer: public QObject
{
    Q_OBJECT
public:
    CapturedLayer(QImage*);
    void setCapturedRect(const QRect &rect);
    void paint(QPainter &painter);
    void mouseDoubleClickEvent(QMouseEvent *);
    ~CapturedLayer(){}
signals:
    void saveCapturedRectSignal(QRect *, CapturedRectSaveType saveType);
private:
    QImage *screen_pic_;
    QRect captured_rect_;
};

#endif // CAPTURED_LAYER_H
