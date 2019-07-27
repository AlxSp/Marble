#include "ncpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Nucleus {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		NC_CORE_ASSERT(false, "Unknown error!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:
				NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices, count);
		}

		NC_CORE_ASSERT(false, "Unknown RenderAPI error!");
		return nullptr;
	}
}