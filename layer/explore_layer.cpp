#include "layer/explore_layer.h"
#include "helper/math_helper.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QSize>

// 工具面板的size
const int TOOL_PANE_WIDTH = 201;
const int TOOL_PANE_HEIGHT = 351;

ExploreLayer::ExploreLayer(QImage *screenPic, QSize canvasSize)
    : screen_pic_(screenPic), canvas_size_(canvasSize), mouse_pos_(QPoint()) {}

ExploreLayer::~ExploreLayer() {}

void ExploreLayer::paint(QPainter &painter) {
  painter.save();
  // 绘制鼠标所在的位置垂直和水平线
  painter.setPen(QPen(Qt::red, 1));
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 垂直线
  painter.drawLine(mX, 0, mX, this->canvas_size_.height());
  // 水平线
  painter.drawLine(0, mY, this->canvas_size_.width(), mY);

  this->paintToolPane(painter);

  painter.restore();
}

void ExploreLayer::mouseMoveEvent(QMouseEvent *event) {
  this->mouse_pos_ = event->pos();
}
void ExploreLayer::keyMoveEvent(const int key) {
  int dx = 0;
  int dy = 0;
  if (key == Qt::Key_Right) {
    dx = 1;
  } else if (key == Qt::Key_Left) {
    dx = -1;
  } else if (key == Qt::Key_Down) {
    dy = 1;
  } else if (key == Qt::Key_Up) {
    dy = -1;
  }
  auto pos = &this->mouse_pos_;
  pos->setX(pos->x() + dx);
  pos->setY(pos->y() + dy);
}

/**
 * 渲染一个工具面板，包含：
 * 鼠标位置周围的图像
 * 鼠标位置信息等
 * 鼠标所在位置的颜色信息
 * @param painter
 */
void ExploreLayer::paintToolPane(QPainter &painter) {
  // 鼠标位置
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 画布尺寸
  QSize canvasSize = this->canvas_size_;
  // 工具面板默认放置画布于右下角，
  // 但如果鼠标移入了右下角区域，为了防止挡住，工具面板位置变为左上角
  int panelX = canvasSize.width() - TOOL_PANE_WIDTH;
  int panelY = canvasSize.height() - TOOL_PANE_HEIGHT;
  if (mX >= panelX && mY >= panelY) {
    // 进入被遮挡区域
    panelX = 0;
    panelY = 0;
  }
  this->paintToolPaneAt(panelX, panelY, painter);
}

/**
 * 在任意一个位置绘制工具面板
 * @param panelX
 * @param panelY
 * @param painter
 */
void ExploreLayer::paintToolPaneAt(int panelX, int panelY, QPainter &painter) {
  painter.save();

  // 1. 准备工具面板基本Rect数据
  // 注意，这里我们不考虑缩放问题
  const int TOOL_PANE_X = panelX;
  const int TOOL_PANE_Y = panelY;
  const QRect TOOL_PANE_RECT(TOOL_PANE_X, TOOL_PANE_Y, TOOL_PANE_WIDTH,
                                   TOOL_PANE_HEIGHT);

  // 2. 计算鼠标的实际像素位置
  int mX = this->mouse_pos_.x();
  int mY = this->mouse_pos_.y();
  // 由于mouse_pos是逻辑坐标，为了获取图片上对应的值，需要转为实际坐标
  auto picRealSize = this->screen_pic_->size();
  int realMouseX = mX * picRealSize.width() / canvas_size_.width();
  int realMouseY = mY * picRealSize.height() / canvas_size_.height();
  
  // 3. 所在图片真实像素位置周围30px的区域内容
  // 需要将对应区域转为实际区域，进而获取实际的区域的图像内容
  const int PADDING = 30;
  int left = realMouseX - PADDING;
  int top = realMouseY - PADDING;
  // +1是因为要把鼠标所在位置的1个像素加上
  int w = PADDING * 2 + 1;
  int h = PADDING * 2 + 1;
  QRect realPicRect = QRect(left, top, w, h);
  // 从中取得对应区域的图像内容
  QImage partialImage = this->screen_pic_->copy(realPicRect);

  // 获取鼠标指定位置的颜色
  QRgb rgbAtMousePos = this->screen_pic_->pixel(realMouseX, realMouseY);

  // === 绘制阶段 ===
  // === 1. 给工具面板绘制1像素的边框 ===
  // 注意绘制边框的时候，如果取值为>=2，需要考虑宽度带来的偏移问题，
  // 因为drawRect的线条是pen的中心像素向两边扩展，为了实现方便建议就用1
  const int BORDER_W = 1;
  painter.setPen(QPen(QColor(0, 0, 0), BORDER_W));
  // 先使用白色将整个面板填满
  painter.fillRect(TOOL_PANE_RECT, QBrush(Qt::white));
  // 再绘制边框
  painter.drawRect(TOOL_PANE_RECT);

  // === 2. 面板顶部绘制一个存放放大图像的区域 ===
  const int SCALED_IMG_W = TOOL_PANE_WIDTH - (BORDER_W * 2);
  const int SCALED_IMG_H = SCALED_IMG_W;
  const int SCALED_IMG_X = TOOL_PANE_X + BORDER_W;
  const int SCALED_IMG_Y = TOOL_PANE_Y + BORDER_W;
  painter.drawImage(
      QRect(SCALED_IMG_X, SCALED_IMG_Y, SCALED_IMG_W, SCALED_IMG_H),
      partialImage);

  // === 3. 在图像放大区域绘制一个中央十字 ===
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

  // === 4. 在下方涂写十字中心捕获的颜色 === 
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
  // 使用反色
  painter.setPen(QPen(Qt::white, 1));
  painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
  painter.drawText(colorDisplayRect, Qt::AlignVCenter | Qt::AlignRight,
                   rgbDesc);
  // 恢复颜色
  painter.setCompositionMode(QPainter::CompositionMode_Source);

  // === 5. 在色彩卡下方绘制坐标，以及RGB文字 ===
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
void ExploreLayer::setCanvasSize(QSize size) { this->canvas_size_ = size; }
