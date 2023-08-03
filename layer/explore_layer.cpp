
#include "layer/explore_layer.h"
#include "../helper/paint_helper.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QSize>

ExploreLayer::ExploreLayer(QImage *screenPic, QSize screenSize,
                           QSize screenScale)
    : screen_pic_(screenPic), screen_size_(screenSize),
      screen_scale_(screenScale), mouse_pos_(QPoint()),
      assistant_panel_size_(201, 351) {}

ExploreLayer::~ExploreLayer() {}

void ExploreLayer::paint(QPainter &painter) {
  painter.save();
  // 绘制鼠标所在的位置垂直和水平线
  painter.setPen(QPen(Qt::red, 1));
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 垂直线
  painter.drawLine(mX, 0, mX, this->screen_size_.height());
  // 水平线
  painter.drawLine(0, mY, this->screen_size_.width(), mY);

  this->paintAssistantPanel(painter);

  painter.restore();
}

void ExploreLayer::mouseMoveEvent(QMouseEvent *event) {
  this->mouse_pos_ = event->pos();
}

/**
 * @brief ExploreLayer::paintAssistantArea
 * 渲染右下角的颜色捕获
 * @param painter
 */
void ExploreLayer::paintAssistantPanel(QPainter &painter) {
  // 鼠标位置
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 屏幕尺寸
  QSize screenSize = this->screen_size_;
  // 辅助面板默认放置于右下角，
  // 但如果鼠标移入了右下角区域，为了防止挡住，辅助面板位置变为左上角
  int panelX = screenSize.width() - this->assistant_panel_size_.width();
  int panelY = screenSize.height() - this->assistant_panel_size_.height();
  if (mX >= panelX && mY >= panelY) {
    // 进入被遮挡区域
    panelX = 0;
    panelY = 0;
  }
  this->paintAssistantPanelAt(panelX, panelY, painter);
}

/**
 * @brief ExploreLayer::paintAssistantAreaAt
 * 在任意一个位置绘制辅助面板
 * @param panelX
 * @param panelY
 * @param painter
 */
void ExploreLayer::paintAssistantPanelAt(int panelX, int panelY,
                                         QPainter &painter) {
  painter.save();

  // 1 基础数据
  // 鼠标位置
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 由于mouse_pos是逻辑坐标，为了获取图片上对应的值，需要转为实际坐标
  int realX = mX * this->screen_scale_.width();
  int realY = mY * this->screen_scale_.height();
  // 获取鼠标指定位置的颜色
  QRgb rgbAtMousePos = this->screen_pic_->pixel(realX, realY);

  // 2 鼠标所在位置周围30px（逻辑像素）的区域内容
  const int PADDING = 30;
  int left = mX - PADDING;
  int top = mY - PADDING;
  // +1是因为要把鼠标所在位置的1个像素加上
  QRect logicRect = QRect(left, top, PADDING * 2 + 1, PADDING * 2 + 1);
  // 将逻辑像素下的Rect转为真实图像上的Rect
  QRect realPicRect = paint_helper::rectLogicPixelToRealPixel(
      logicRect, this->screen_scale_.width(), this->screen_scale_.height());

  // 从中取得对应区域的图像内容
  QImage partialImage = this->screen_pic_->copy(realPicRect);

  // 3 辅助面板矩形数据
  const int ASSISTANT_PANEL_W = this->assistant_panel_size_.width();
  const int ASSISTANT_PANEL_H = this->assistant_panel_size_.height();
  const int ASSISTANT_PANEL_X = panelX;
  const int ASSISTANT_PANEL_Y = panelY;
  const QRect ASSISTANT_PANEL_RECT =
      QRect(ASSISTANT_PANEL_X, ASSISTANT_PANEL_Y, ASSISTANT_PANEL_W,
            ASSISTANT_PANEL_H);

  // 3.1 绘制1像素的边框
  // 注意绘制边框的时候，如果取值为>=2，需要考虑宽度带来的偏移问题，
  // 因为drawRect的线条是pen的中心像素向两边扩展，为了实现方便建议就用1
  const int BORDER_W = 1;
  painter.setPen(QPen(QColor(0, 0, 0), BORDER_W));
  // 先使用白色将整个面板填满
  painter.fillRect(ASSISTANT_PANEL_RECT, QBrush(Qt::white));
  // 再绘制边框
  painter.drawRect(ASSISTANT_PANEL_RECT);

  // 3.2 绘制边框内部的对应经过放大的图片
  const int SCALED_IMG_W = ASSISTANT_PANEL_W - (BORDER_W * 2);
  const int SCALED_IMG_H = SCALED_IMG_W;
  const int SCALED_IMG_X = ASSISTANT_PANEL_X + BORDER_W;
  const int SCALED_IMG_Y = ASSISTANT_PANEL_Y + BORDER_W;
  painter.drawImage(
      QRect(SCALED_IMG_X, SCALED_IMG_Y, SCALED_IMG_W, SCALED_IMG_H),
      partialImage);

  // 3.3 绘制放大的图片中央的十字
  // SCALED_IMG_W为奇数
  const int IMG_CENTER_POINT_X = SCALED_IMG_X + (SCALED_IMG_W - 1) / 2;
  const int IMG_CENTER_POINT_Y = SCALED_IMG_Y + (SCALED_IMG_W - 1) / 2;
  // 设置反色模式
  // 需要将Pen的颜色设置为white，然后使用光栅的颜色叠加（AndNot）
  painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
  painter.setPen(QPen(Qt::white, 2));
  // 设置十字路径并绘制
  QPainterPath path;
  const int EXPAND_LENGTH = 20;
  path.moveTo(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y - EXPAND_LENGTH);
  path.lineTo(IMG_CENTER_POINT_X, IMG_CENTER_POINT_Y + EXPAND_LENGTH);
  path.moveTo(IMG_CENTER_POINT_X - EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  path.lineTo(IMG_CENTER_POINT_X + EXPAND_LENGTH, IMG_CENTER_POINT_Y);
  painter.drawPath(path);
  // 绘制反色十字完成后，一定要将反色模式取消，
  // 否则后续涂色的时候，在白色背景上一直是黑色
  painter.setCompositionMode(QPainter::CompositionMode_Source);

  // 4. 在下方涂写十字中心捕获的颜色
  // 4.1 区域中填充颜色
  QBrush brush(rgbAtMousePos);
  const int COLOR_DISPLAY_RECT_W = SCALED_IMG_W;
  const int COLOR_DISPLAY_RECT_H = 25;
  const int COLOR_DISPLAY_RECT_X = SCALED_IMG_X;
  const int COLOR_DISPLAY_RECT_Y = SCALED_IMG_Y + SCALED_IMG_H + BORDER_W;
  QRect colorDisplayRect(COLOR_DISPLAY_RECT_X, COLOR_DISPLAY_RECT_Y,
                         COLOR_DISPLAY_RECT_W, COLOR_DISPLAY_RECT_H);
  painter.fillRect(colorDisplayRect, QBrush(rgbAtMousePos));
  // 4.2 使用反色，在区域右侧填充色彩信息
  const QColor color(rgbAtMousePos);
  const QString rgbDesc = QString("rgb(%1, %2, %3)")
                              .arg(color.red(), 3, 10, QChar('0'))
                              .arg(color.green(), 3, 10, QChar('0'))
                              .arg(color.blue(), 3, 10, QChar('0'));
  // 再次使用反色
  painter.setPen(QPen(Qt::white, 1));
  painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
  painter.drawText(colorDisplayRect, Qt::AlignVCenter | Qt::AlignRight, rgbDesc);
  // 恢复颜色
  painter.setCompositionMode(QPainter::CompositionMode_Source);

  // 5. 在色彩卡下方绘制坐标，以及RGB文字
  // 5.1 准备好区域
  const int INFO_DESC_RECT_W = SCALED_IMG_W;
  const int INFO_DESC_RECT_H = 40;
  const int INFO_DESC_RECT_X = SCALED_IMG_X;
  const int INFO_DESC_RECT_Y = COLOR_DISPLAY_RECT_Y + COLOR_DISPLAY_RECT_H;
  QRect infoDescRect(INFO_DESC_RECT_X, INFO_DESC_RECT_Y, INFO_DESC_RECT_W,
                     INFO_DESC_RECT_H);
  // 5.2 准备好文字
  const QString posDesc = QString("x = %1, y = %2")
                              .arg(QString::number(mX))
                              .arg(QString::number(mY));
  painter.setPen(QPen(Qt::black));
  painter.drawText(infoDescRect, Qt::AlignHCenter | Qt::AlignVCenter, posDesc);

  // finally
  painter.restore();
}
