#include "ncpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Nucleus {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
		}

		NC_CORE_ASSERT(false, "Unknown error!");
		return nullptr;
	}
}