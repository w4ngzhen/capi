cmake_minimum_required(VERSION 3.5)

project(QtScreenShot LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置QT路径
# 在macOS，它可能是这样:"/Users/w4ngzhen/Qt/6.2.4/macos"
# 在Windows，它可能是这样:"D:\\Qt\\6.2.4\\msvc2019_64"
# 这里为了统一，建议不同的操作系统将Qt对应工具链的路径写入到环境变量中
# 这里统一读取环境变量 QT_DEV_DIR
if(NOT DEFINED ENV{QT_DEV_DIR})
  message(FATAL_ERROR ">>> not define 'QT_DEV_DIR' in env path, please add it. <<<")
else()
  set(CMAKE_PREFIX_PATH $ENV{QT_DEV_DIR})
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(QtScreenShot WIN32
  main.cpp
  screenshot_widget.cpp
  screenshot_widget.h
  screenshot_status.h
  helper/math_helper.h
  helper/math_helper.cpp
  helper/paint_helper.h
  helper/paint_helper.cpp
  layer/explore_layer.h
  layer/explore_layer.cpp
  layer/capturing_layer.h
  layer/capturing_layer.cpp
  layer/captured_layer.h
  layer/captured_layer.cpp
)

target_link_libraries(QtScreenShot PRIVATE Qt6::Widgets)
