cmake_minimum_required(VERSION 2.8)
project(Zephyr CXX)

# We use C++11
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
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
    ${SRC}/glfw/init.cpp
    ${SRC}/gfx/Window.cpp
    ${SRC}/gfx/BufferSwapper.cpp
    src/Zephyr.cpp)

    
include_directories("${PROJECT_BINARY_DIR}/src")

# Required libraries. Order matters!
target_link_libraries(game GLEW GLU glfw3 GL X11 Xxf86vm Xrandr pthread Xi)

add_executable(demo
    example/example.cpp)

target_link_libraries(demo GL GLEW glfw)


# Unit testing
enable_testing()

# Need to include gmock as it is
add_subdirectory(gmock)
include_directories(gmock/include gmock/gtest/include) 

# Unit test executable
add_executable(runUnitTests
    test/tests.cpp
    test/failing.cpp
)

target_link_libraries(runUnitTests gmock gmock_main)

# Test definitions
add_test(
    NAME someTest
    COMMAND runUnitTests
)
