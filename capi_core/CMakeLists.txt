
project(capi_core LANGUAGES CXX)

set(CAPI_CORE_HEADERS
    include/capi_core/base/point.h
    include/capi_core/base/rect.h
    include/capi_core/base/size.h
    include/capi_core/event/captured_image_save_event.h
    include/capi_core/layer/captured_layer.h
    include/capi_core/layer/captured_layer_common.h
    include/capi_core/layer/capturing_layer.h
    include/capi_core/layer/explore_layer.h
    include/capi_core/layer/layer.h
    include/capi_core/paint/brush.h
    include/capi_core/paint/color.h
    include/capi_core/paint/image.h
    include/capi_core/paint/painter.h
    include/capi_core/paint/pen.h
    include/capi_core/platform/platform.h
    include/capi_core/platform/platform_shape_config_pane.h
    include/capi_core/utils/math_utils.h
    include/capi_core/canvas.h
    include/capi_core/canvas_status.h
    include/capi_core/global.h
    include/capi_core/layer/shape/captured_shape.h
    include/capi_core/layer/shape/ellipse_shape.h
    include/capi_core/layer/shape/line_shape.h
    include/capi_core/layer/shape/rect_shape.h
    include/capi_core/layer/shape/shape.h
    include/capi_core/layer/shape/shape_config.h
    include/capi_core/layer/shape/shape_config_pane.h
)

set(CAPI_CORE_SOURCES
    canvas.cpp
    utils/math_utils.cpp
    platform/platform_shape_config_pane.cpp
    paint/pen.cpp
    paint/painter.cpp
    paint/image.cpp
    paint/color.cpp
    paint/brush.cpp
    layer/shape/captured_shape.cpp
    layer/shape/ellipse_shape.cpp
    layer/shape/line_shape.cpp
    layer/shape/rect_shape.cpp
    layer/shape/shape.cpp
    layer/shape/shape_config.cpp
    layer/shape/shape_config_pane.cpp
    layer/captured_layer.cpp
    layer/capturing_layer.cpp
    layer/explore_layer.cpp
    layer/layer.cpp
    event/captured_image_save_event.cpp
    base/size.cpp
    base/rect.cpp
    base/point.cpp
)

add_library(
    capi_core
    ${CAPI_CORE_HEADERS}
    ${CAPI_CORE_SOURCES}
)

target_include_directories(capi_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)