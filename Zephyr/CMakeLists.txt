cmake_minimum_required(VERSION 2.8)
project(Zephyr)

set(SRC src/zephyr)

add_executable(game
    ${SRC}/Root.hpp
    ${SRC}/Root.cpp
    src/zephyr/core/Scheduler.hpp
    src/zephyr/core/Scheduler.cpp
    src/zephyr/core/Task.hpp
    src/zephyr/core/Task.cpp
    src/zephyr/core/Config.hpp
    src/zephyr/core/Config.cpp
    src/example/Speaker.hpp
    src/Zephyr.cpp)


add_executable(demo
    example/example.cpp)

target_link_libraries(demo GL GLEW glfw)

include_directories("${PROJECT_BINARY_DIR}/src")

add_definitions(-std=c++11)
