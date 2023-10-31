cmake_minimum_required(VERSION 3.5)

project(capi_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(capi_core)
add_subdirectory(capi_impl_qt)