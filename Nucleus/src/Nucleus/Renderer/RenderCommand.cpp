#include "ncpch.h"
#include "Nucleus/Renderer/RenderCommand.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Nucleus {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}