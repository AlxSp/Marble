#include "mblpch.h"

#include "Marble/Renderer/VertexArray.h"
#include "Marble/Renderer/Renderer.h"

#include "RendererAPI/OpenGL/OpenGLVertexArray.h"

namespace Marble {

	Ref<VertexArray> VertexArray::Create() {

		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None: MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		MBL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}