#include "mblpch.h"
#include "Marble/Renderer/RenderCommand.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Marble {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}