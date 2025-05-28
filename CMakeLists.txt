cmake_minimum_required(VERSION 3.31)
project(typish)

set(CMAKE_CXX_STANDARD 20)

include_directories($ENV{OPENGL_PATH}/include)
link_directories($ENV{OPENGL_PATH}/lib)

add_executable(typish main.cpp)

target_link_libraries(typish glfw3 opengl32 glew32)
