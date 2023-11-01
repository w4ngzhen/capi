project(capi_impl_sdl LANGUAGES CXX)

# 查询SDL2模块
find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED COMPONENTS SDL2_image)

if (APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework AppKit -framework CoreData -framework Foundation -framework ImageIO")
endif ()

add_executable(
    capi_impl_sdl
    WIN32
    MACOSX_BUNDLE
    main.cpp
)

target_include_directories(
    capi_impl_sdl
    PRIVATE
    ./
    ../capi_core/include
)

add_dependencies(
    capi_impl_sdl
    capi_core
)

target_link_libraries(
    capi_impl_sdl
    PRIVATE
    capi_core
    SDL2::SDL2
    SDL2_image::SDL2_image
)
