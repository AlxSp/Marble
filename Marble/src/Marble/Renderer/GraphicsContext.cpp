#include "mblpch.h"
#include "Marble/Renderer/Renderer.h"
#include "Marble/Renderer/GraphicsContext.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLContext.h"

namespace Marble {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI()) 
        {
            case RendererAPI::API::None: MBL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        MBL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
