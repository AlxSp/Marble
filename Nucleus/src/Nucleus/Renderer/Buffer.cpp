#include "ncpch.h"
#include "Nucleus/Renderer/Buffer.h"
#include "Nucleus/Renderer/Renderer.h"
//RendererAPI Headers
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Nucleus {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size, type);
		}

		NC_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size, type);
		}

		NC_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	NC_CORE_ASSERT(false, "RendererAPI::None is currently not supported");	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count, type);
		}

		NC_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}