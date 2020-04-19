#include "mblpch.h"
#include "Marble/Renderer/RendererAPI.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Marble {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: MBL_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
		}

		MBL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}