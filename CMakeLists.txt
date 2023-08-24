cmake_minimum_required(VERSION 3.5)

project(capi LANGUAGES CXX)

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
if (NOT DEFINED ENV{QT_DEV_DIR})
  message(FATAL_ERROR ">>> not define 'QT_DEV_DIR' in env path, please add it. <<<")
else ()
  set(CMAKE_PREFIX_PATH $ENV{QT_DEV_DIR})
endif ()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 将src下面的所有头文件路径保存至 all_head_files 数组变量中
# 将src下面的所有源文件路径保存至 all_source_files 数组变量中
file(GLOB_RECURSE all_head_files "src/**/*.hpp" "src/**/*.h")
file(GLOB_RECURSE all_source_files "src/**/*.cpp" "src/**/*.c")

include_directories("src")

add_executable(${CMAKE_PROJECT_NAME} WIN32
    ${all_head_files}
    ${all_source_files}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets)
