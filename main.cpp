#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "QuartzCore/CAMetalLayer.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"
#include "Metal/MTLPixelFormat.hpp"

#include "win.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

int main()
{
    NS::Error* error = nullptr;
    auto device = MTL::CreateSystemDefaultDevice();
    auto commandQueue = device->newCommandQueue();

    //CA::MetalLayer* metalLayer = CA::MetalLayer::layer();
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    int w = 800;
    int h = 600;
    auto* glfwWindow = glfwCreateWindow(w, h, "Metallia", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    CA::MetalLayer* metalLayer = attachMetalLayerToGLFWWindow(glfwWindow, device, w, h);

    //auto library = device->newDefaultLibrary();
    auto libraryPath = NS::String::string("shaders/shaders.metallib", NS::ASCIIStringEncoding);
    auto library = device->newLibrary(libraryPath, &error);
    printf("error %s", error->localizedDescription()->utf8String());
    
    auto vertexFunction = library->newFunction(NS::String::string("vertex_main", NS::ASCIIStringEncoding));
    auto fragmentFunction = library->newFunction(NS::String::string("fragment_main", NS::ASCIIStringEncoding));

    MTL::RenderPipelineDescriptor* pipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    pipelineDescriptor->setVertexFunction(vertexFunction);
    pipelineDescriptor->setFragmentFunction(fragmentFunction);
    pipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);

    auto pipelineState = device->newRenderPipelineState(pipelineDescriptor, &error);

    auto commandBuffer = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    renderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));
    renderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    renderPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
 
    float vertexData[] = {
        -0.25f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };

    auto renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setRenderPipelineState(pipelineState);
    renderEncoder->setVertexBytes(vertexData, sizeof(vertexData), 0);
    //renderEncoder->setViewport(MTL::Viewport{0.0, 0.0, (float)w, (float)h, 0.0, 1.0});
    renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    renderEncoder->endEncoding();

    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        
        CA::MetalDrawable* cametalDrawable = (CA::MetalDrawable*)metalLayer->nextDrawable();
        if (!cametalDrawable) {
            continue;
        }
        
        MTL::Texture* drawableTexture = cametalDrawable->texture();
        if (!drawableTexture) {
            return -1;
        }

        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        renderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.0, 0.0, 0.0, 1.0));
        renderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        renderPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
        renderPassDescriptor->colorAttachments()->object(0)->setTexture(drawableTexture);
        
        auto commandBuffer = commandQueue->commandBuffer();
        
        auto renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
        glfwGetFramebufferSize(glfwWindow, &w, &h);

        // TODO: something fishy going on here..
        renderEncoder->setViewport(MTL::Viewport{0.0, 0.0, (double)w, (double)h, 0.0, 1.0});

        renderEncoder->setRenderPipelineState(pipelineState);
        renderEncoder->setVertexBytes(vertexData, sizeof(vertexData), 0);
        renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
        renderEncoder->endEncoding();
        
        commandBuffer->presentDrawable(cametalDrawable);
        commandBuffer->commit();
    }

    glfwDestroyWindow(glfwWindow);
    glfwTerminate();

    return 0;
}


