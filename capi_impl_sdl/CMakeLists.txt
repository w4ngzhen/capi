project(capi_impl_sdl LANGUAGES CXX)

# 查询SDL2模块
find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED COMPONENTS SDL2_image)

# =====  imgui  =====
# imgui header
set(IMGUI_HEADER_DIR
    ../3rdparty/imgui
    ../3rdparty/imgui/backends)
# imgui sources
set(IMGUI_SOURCE_FILES
    ../3rdparty/imgui/imgui.cpp
    ../3rdparty/imgui/imgui_demo.cpp
    ../3rdparty/imgui/imgui_draw.cpp
    ../3rdparty/imgui/imgui_tables.cpp
    ../3rdparty/imgui/imgui_widgets.cpp
    ../3rdparty/imgui/backends/imgui_impl_sdlrenderer2.cpp
    ../3rdparty/imgui/backends/imgui_impl_sdl2.cpp
)

# ==== cross_platform_screenshot =====
add_subdirectory(
    ../3rdparty/cross_platform_screenshot/cp_screenshot_core
    cp_screenshot_core
)
set(CP_SCREENSHOT_HEADER_DIR ../3rdparty/cross_platform_screenshot/cp_screenshot_core/include)

if (APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework AppKit -framework CoreData -framework Foundation -framework ImageIO")
endif ()

add_executable(
    capi_impl_sdl
    WIN32
    MACOSX_BUNDLE
    main.cpp
    ${IMGUI_SOURCE_FILES}
)

target_include_directories(
    capi_impl_sdl
    PRIVATE
    ./
    ../capi_core/include
    # imgui header dir
    ${IMGUI_HEADER_DIR}
    # cross_platform_screenshot header dir
    ${CP_SCREENSHOT_HEADER_DIR}
)

add_dependencies(
    capi_impl_sdl
    capi_core
    cp_screenshot_core
)

target_link_libraries(
    capi_impl_sdl
    PRIVATE
    capi_core
    cp_screenshot_core
    SDL2::SDL2
    SDL2_image::SDL2_image
)
