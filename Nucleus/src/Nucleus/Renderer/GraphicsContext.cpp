#include "ncpch.h"
#include "Nucleus/Renderer/Renderer.h"
#include "Nucleus/Renderer/GraphicsContext.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLContext.h"

namespace Nucleus {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI()) 
        {
            case RendererAPI::API::None: NC_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        NC_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
