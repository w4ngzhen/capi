#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ScreenShotWidget; }
QT_END_NAMESPACE

class ScreenShotWidget : public QWidget
{
    Q_OBJECT

public:
    ScreenShotWidget(QWidget *parent = nullptr);
    ~ScreenShotWidget();
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::ScreenShotWidget *ui;


};
#endif // SCREENSHOTWIDGET_H
