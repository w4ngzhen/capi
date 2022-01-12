#include "screenshotwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenShotWidget w;
    w.show();
    return a.exec();
}
