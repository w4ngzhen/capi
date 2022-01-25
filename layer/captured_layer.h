#ifndef CAPTURED_LAYER_H
#define CAPTURED_LAYER_H

#include <QMouseEvent>
#include <QRect>
#include <QSize>
class QPainter;

enum CapturedRectSaveType
{
    ToClipboard,
    ToFile
};

class CapturedLayer: public QObject
{
    Q_OBJECT
public:
    CapturedLayer(QSize screenSize);
    void setCapturedRect(const QRect &rect);
    void setScreenSize(const QSize &size);
    void paint(QPainter &painter);
    void mouseDoubleClickEvent(QMouseEvent *);
    ~CapturedLayer(){}
signals:
    void saveCapturedRectSignal(QRect *, CapturedRectSaveType saveType);
private:
    QSize screen_size_;
    QRect captured_rect_;
};

#endif // CAPTURED_LAYER_H
