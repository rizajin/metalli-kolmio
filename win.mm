#include "win.h"

#import <AppKit/AppKit.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.hpp>


CA::MetalLayer* attachMetalLayerToGLFWWindow(GLFWwindow* glfwWindow, MTL::Device* device, int width, int height) {
    NSWindow* nsWindow = reinterpret_cast<NSWindow*>(glfwGetCocoaWindow(glfwWindow));
    NSView* nsView = [nsWindow contentView];
    CA::MetalLayer* metalLayer = CA::MetalLayer::layer();
    metalLayer->setDevice(device);
    metalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    metalLayer->setDrawableSize(CGSizeMake(width, height));

    nsView.layer = (CALayer*)metalLayer;
    [nsView setWantsLayer:YES];

    return metalLayer;
}
