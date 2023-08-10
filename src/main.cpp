#include "canvas_widget.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // 获取鼠标所在的屏幕
  QScreen *screen = QApplication::screenAt(QCursor().pos());
  QImage screenImg = screen->grabWindow(0).toImage();

  // CanvasWidget内部屏蔽了"屏幕图像"这一概念
  // 只知道将一张图绘制到窗体，然后进行截图操作
  CanvasWidget w;
  w.init(&screenImg);
  w.showFullScreen();

  auto code = a.exec();

  delete screen;
  return code;
}
