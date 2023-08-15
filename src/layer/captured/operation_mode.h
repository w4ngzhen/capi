
#ifndef OPERATION_MODE_H
#define OPERATION_MODE_H

enum OperationMode {
  Normal = 0,

  DraggingLeftTop = 0x01,
  DraggingRightTop = 0x02,
  DraggingLeftBottom = 0x03,
  DraggingRightBottom = 0x04,
  DraggingArea = 0x10,

  EnableDrawRect = 0x20,
  DrawingRect = 0x21,
};

#endif
