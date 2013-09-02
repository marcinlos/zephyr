/**
 * @file init.cpp
 *
 * Initialization code for GLFW.
 */

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// anonymous namespace
namespace {

/**
 * Error callback for GLFW framework.
 */
void errorCallback(int code, const char* description) {
    std::cerr << "[GLFW] Error " << code << ": " << description << std::endl;
}

/**
 * Plain old static object initializer. Constructor contains initialization
 * code, destructor handles the cleanup. As long as the order of various
 * components does not matter, it should be enough.
 */
struct InitGLFW {

    InitGLFW() {
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit()) {
            std::cerr << "[GLFW] Initialization error!" << std::endl;
            std::cerr << "[GLFW] Aborting..." << std::endl;
            std::exit(1);
        } else {
            std::cout << "[GLFW] Initialized" << std::endl;
        }
    }

    ~InitGLFW() {
        std::cout << "[GLFW] Shutting down..." << std::endl;
        glfwTerminate();
        std::cout << "[GLFW] Done" << std::endl;
    }
}
// static global object
init;

}
