#pragma once

#include <vector>
#include <functional>
#include "core/global/global.h"
#include "core/layer/layer.h"
#include "core/event/captured_image_save_event.h"
#include "core/layer/captured/shape/shape_config.h"
#include "captured_layer_common.h"
#include "core/layer/captured/shape/shape.h"

namespace capi {

/**
 * 事件回调EventCb
 * 区域捕获完成，并处理完成事件
 */
typedef std::function<void(const CapturedImageSaveEvent *)>
    LayerEventOnCapturedLayerImageSaveCb;

enum ShapeType {
  Rectangle = 0,
  Ellipse
};

class CapturedLayer : public Layer {
public:
  /**
   * 构造函数
   */
  explicit CapturedLayer(const Size &canvasSize);
  /**
   * 设置捕获矩形信息给内部的CapturedShape（如有）
   */
  void setCapturedRect(const Rect &capturedRect);
  /**
  * 增加指定类型，指定配置的图形
  * @param selected 增加后是否立即选中
  */
  void addShape(ShapeType, const ShapeConfig &, bool selected);
  /**
  * 移除正在选择的图形
  */
  void deleteSelectedShape();
  /**
 * 获取CapturedShape
 */
  Shape *getCapturedShape();
  void onPaint(Painter *) override;
  void onMousePress(const Point &) override;
  void onMouseMove(const Point &) override;
  void onMouseRelease(const Point &) override;
  void onMouseDoubleClick(const Point &) override;
  void onKeyPress(Key, KeyboardModifier) override;
  void onCanvasResize(const Size &size) override;
  /**
  * 设置图片捕获处理后的保存事件
  */
  void setLayerEventOnCapturedLayerImageSaveCb(LayerEventOnCapturedLayerImageSaveCb cb);
private:
  /**
   * 使用鼠标点击选择某个图形
   * @param mousePos
   */
  void selectShape(const Point &mousePos);
  /**
   * 使用鼠标悬浮到某个图形
   * @param mousePos
   */
  void hoverShape(const Point &mousePos);
  /**
   * 将正在选择的图形移动到指定图形
   * 注意：
   * CapturedShape 始终处于图层索引为0的位置
   * 无法移动 CapturedShape，以及将任何其他的图形移动到 CapturedShape 的下层
   *
   * @param levelIdx
   */
  void moveSelectedShapeToTargetLevel(int levelIdx);
  /**
   * 已有的所有图形
   */
  std::vector<Shape *> shapes_;
  /**
   * 存储当前正被选中的图形
   */
  Shape *selected_shape_ = nullptr;
  /**
   * 描述当前被选择图形正被拖动的部分
   * 根据该值决定鼠标按下、移动等操作达到对图形拖动的效果
   */
  ShapePart selected_shape_dragging_part_ = None;
  /**
  * 捕获完成、处理后的保存回调
  */
  LayerEventOnCapturedLayerImageSaveCb layer_event_on_captured_layer_image_save_cb_ = nullptr;
};
}
