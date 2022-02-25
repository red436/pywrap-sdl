cmake_minimum_required(VERSION 3.4...3.18)
project(example LANGUAGES CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(pybind11 REQUIRED)  # or add_subdirectory(pybind11)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(test main.cpp imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_sdl.cpp imgui/backends/imgui_impl_sdlrenderer.cpp)
target_link_libraries(test PRIVATE pybind11::embed ${SDL2_LIBRARIES} SDL2_image SDL2_gfx box2d )
target_include_directories(test PUBLIC "${PROJECT_BINARY_DIR}" ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
