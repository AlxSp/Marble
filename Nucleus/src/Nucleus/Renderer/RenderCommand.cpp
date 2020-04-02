#include "ncpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Nucleus {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}