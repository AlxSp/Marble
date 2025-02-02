#include "mblpch.h"
#include "Marble/Renderer/Buffer.h"
#include "Marble/Renderer/Renderer.h"
//RendererAPI Headers
#include "RendererAPI/OpenGL/OpenGLBuffer.h"

namespace Marble {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size, type);
		}

		MBL_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size, type);
		}

		MBL_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count, BufferType type) 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count, type);
		}

		MBL_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create() 
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::None:	MBL_CORE_ASSERT(false, "RendererAPI::None is currently not supported");	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>();
		}

		MBL_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}