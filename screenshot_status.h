#ifndef SCREENSHOT_STATUS_H
#define SCREENSHOT_STATUS_H

enum  ScreenShotStatus
{

    // 鼠标悬浮移动
    Explore = 0,
    // 鼠标按下后拖动选择中
    Capturing,
    // 鼠标松开捕获选择图片中
    Captured
};

#endif // SCREENSHOT_STATUS_H
