#include "ncpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
//#include "Platform/OpenGL/OpenGlRendererAPI.h"

namespace Nucleus {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}