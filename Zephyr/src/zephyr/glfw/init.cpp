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

struct InitGLFW
{
    InitGLFW() 
    { 
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
