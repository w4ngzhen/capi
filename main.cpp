#include "screenshot_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ScreenShotWidget w;
    w.showFullScreen();

    return a.exec();
}
