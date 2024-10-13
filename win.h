#ifndef CREATE_METAL_WINDOW_H
#define CREATE_METAL_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
namespace MTL{
class Device;
};
namespace CA{
class MetalLayer;
};

CA::MetalLayer* attachMetalLayerToGLFWWindow(GLFWwindow* glfwWindow, MTL::Device* device, int width, int height);

#endif