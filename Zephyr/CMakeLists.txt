cmake_minimum_required(VERSION 2.8)
project(Zephyr CXX)

# We use C++11
add_definitions(-std=c++11)

# Coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRC src/zephyr)

add_executable(game
    ${SRC}/core/Scheduler.cpp
    ${SRC}/core/Task.cpp
    ${SRC}/core/MessageQueue.cpp
    ${SRC}/core/MessageDispatcher.cpp
    ${SRC}/core/DispatcherTask.cpp
    ${SRC}/core/Config.cpp
    ${SRC}/glfw/init.cpp
    ${SRC}/glfw/input_adapter.cpp
    ${SRC}/window/Window.cpp
    ${SRC}/window/WindowSystem.cpp
    ${SRC}/input/Key.cpp
    ${SRC}/input/Position.cpp
    ${SRC}/time/ClockManager.cpp
    ${SRC}/gfx/HackyRenderer.cpp
    ${SRC}/Root.cpp
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

set(TSRC test/zephyr)

# Unit test executable
add_executable(runUnitTests
    ${SRC}/core/MessageDispatcher.cpp
    ${SRC}/core/MessageQueue.cpp
    ${SRC}/core/Task.cpp
    ${SRC}/core/DispatcherTask.cpp
    ${SRC}/input/Key.cpp
    ${SRC}/glfw/input_adapter.cpp
    
    ${TSRC}/core/MessageDispatcher_test.cpp
    ${TSRC}/core/MessageQueue_test.cpp
    ${TSRC}/core/DispatcherTask_test.cpp
    ${TSRC}/input/Mod_test.cpp
    ${TSRC}/util/Any_test.cpp
    ${TSRC}/glfw/input_adapter_test.cpp
)

target_link_libraries(runUnitTests gmock gmock_main)

# Test definitions
add_test(
    NAME someTest
    COMMAND runUnitTests
)
