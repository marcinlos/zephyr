cmake_minimum_required(VERSION 2.8)
project(Zephyr CXX)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
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
    src/zephyr/gfx/Window.cpp
    src/zephyr/gfx/BufferSwapper.cpp
    src/example/Speaker.hpp
    src/Zephyr.cpp)

target_link_libraries(game GLEW GLU glfw3 GL X11 Xxf86vm Xrandr pthread Xi )

add_executable(demo
    example/example.cpp)

target_link_libraries(demo GL GLEW glfw)

include_directories("${PROJECT_BINARY_DIR}/src")


add_definitions(-std=c++11)

# Unit testing

add_subdirectory(gtest/)
include_directories(/home/los/bin/gtest-1.7.0/include) 
#link_directories(/home/los/bin/gtest-1.7.0/lib)

enable_testing()

add_executable(runUnitTests
    test/tests.cpp
    test/failing.cpp
)

target_link_libraries(runUnitTests gtest gtest_main)

add_test(
    NAME someTest
    COMMAND runUnitTests
)
