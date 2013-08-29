cmake_minimum_required(VERSION 2.8)
project(Zephyr CXX)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC src/zephyr)

add_executable(game
    ${SRC}/Root.hpp
    ${SRC}/Root.cpp
    ${SRC}/core/Scheduler.hpp
    ${SRC}/core/Scheduler.cpp
    ${SRC}/core/Task.hpp
    ${SRC}/core/Task.cpp
    ${SRC}/core/Config.hpp
    ${SRC}/core/Config.cpp
    ${SRC}/gfx/Window.cpp
    ${SRC}/gfx/BufferSwapper.cpp
    src/example/Speaker.hpp
    src/Zephyr.cpp)

target_link_libraries(game GLEW GLU glfw3 GL X11 Xxf86vm Xrandr pthread Xi)

add_executable(demo
    example/example.cpp)

target_link_libraries(demo GL GLEW glfw)

include_directories("${PROJECT_BINARY_DIR}/src")


add_definitions(-std=c++11)

# Unit testing

add_subdirectory(gmock)
include_directories(gmock/include gmock/gtest/include) 

enable_testing()

add_executable(runUnitTests
    test/tests.cpp
    test/failing.cpp
)

target_link_libraries(runUnitTests gmock gmock_main)

add_test(
    NAME someTest
    COMMAND runUnitTests
)
