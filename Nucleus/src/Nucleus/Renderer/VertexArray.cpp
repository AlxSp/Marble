#include "ncpch.h"

#include "Nucleus/Renderer/VertexArray.h"
#include "Nucleus/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Nucleus {

	Ref<VertexArray> VertexArray::Create() {

		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None: NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		NC_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}