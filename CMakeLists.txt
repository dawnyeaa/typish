cmake_minimum_required(VERSION 3.31)
project(typish)

set(CMAKE_CXX_STANDARD 20)

include_directories(E:/opengl/include)
link_directories(E:/opengl/lib)

add_executable(typish main.cpp)

target_link_libraries(typish glfw3 opengl32 glew32)
