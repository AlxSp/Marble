#include "ncpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Nucleus {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size, BufferType type) {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices, size, type);
		}

		NC_CORE_ASSERT(false, "Unknown error!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count, BufferType type) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLIndexBuffer(indices, count, type);
		}

		NC_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}