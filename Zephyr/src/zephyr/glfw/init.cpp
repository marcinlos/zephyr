/*
 * init.cpp
 * 
 *  Created on: July 30, 2013
 *      Author: los
 */

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// anonymous namespace
namespace
{

void errorCallback(int code, const char* description)
{
    std::cerr << "[Error " << code << "]: " << description << std::endl;
}
    
struct InitGLFW
{
    InitGLFW() 
    { 
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit())
        {
            std::cerr << "GLFW error!" << std::endl;
            std::exit(1);
        }
        else 
        {
            std::cout << "GLFW initialized" << std::endl;
        }
    }
    
    ~InitGLFW() 
    { 
        std::cout <<  "Shutting down GLFW" << std::endl;
        glfwTerminate();
        std::cout <<  "GLFW terminated" << std::endl;
    }
} 
// static global object
init;

}
