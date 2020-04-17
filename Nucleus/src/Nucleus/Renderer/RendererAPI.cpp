#include "ncpch.h"
#include "Nucleus/Renderer/RendererAPI.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Nucleus {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: NC_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		NC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}