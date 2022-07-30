#include "screenshot_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    ScreenShotWidget w;
    w.showFullScreen();
    return a.exec();
}
