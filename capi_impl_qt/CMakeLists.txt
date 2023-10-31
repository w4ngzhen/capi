project(capi_impl_qt LANGUAGES CXX)

# 设置QT路径
# 在macOS，它可能是这样:"/Users/w4ngzhen/Qt/6.2.4/macos"
# 在Windows，它可能是这样:"D:\\Qt\\6.2.4\\msvc2019_64"
# 这里为了统一，建议不同的操作系统将Qt对应工具链的路径写入到环境变量中
# 这里统一读取环境变量 QT_TOOLKIT_DIR
if (NOT DEFINED ENV{QT_TOOLKIT_DIR})
  message(FATAL_ERROR ">>> not define 'QT_TOOLKIT_DIR' in env path, please add it. <<<")
else ()
  set(CMAKE_PREFIX_PATH $ENV{QT_TOOLKIT_DIR})
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  find_package(Qt6 COMPONENTS Widgets REQUIRED)
endif ()

add_executable(
    capi_impl_qt
    WIN32
    MACOSX_BUNDLE
    main.cpp
    canvas_widget.h
    canvas_widget.cpp
    image_qt_impl.h
    image_qt_impl.cpp
    painter_qt_impl.h
    painter_qt_impl.cpp
)

target_include_directories(
    capi_impl_qt
    PRIVATE
    ./
    ../capi_core/include
)

add_dependencies(
    capi_impl_qt
    capi_core
)

target_link_libraries(
    capi_impl_qt
    PRIVATE
    Qt6::Widgets
    capi_core
)
